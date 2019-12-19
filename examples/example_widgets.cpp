#include <carnot>
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <Engine/ImGui/imgui_internal.h>

using namespace carnot;

///////////////////////////////////////////////////////////////////////////////

namespace ImGui {

void RenderGrid(ImRect bb, int nx, int ny, ImU32 gridColor, ImU32 bgColor, float thickness = 1.0f) {
    ImDrawList* DrawList = GetWindowDrawList();
    float dx = bb.GetWidth() / nx;
    float dy = bb.GetHeight() / ny;  
    DrawList->AddRectFilled(bb.Min, bb.Max, bgColor);
    for (int ix = 0; ix < nx + 1; ++ix) {
        ImVec2 t = bb.Min + ImVec2(ix * dx, 0);
        ImVec2 b = t; b.y += bb.GetHeight();
        DrawList->AddLine(t,b,gridColor,thickness);
    }
    for (int iy = 0; iy < ny + 1; ++iy) {
        ImVec2 l = bb.Min + ImVec2(0, iy * dy);
        ImVec2 r = l; l.x += bb.GetWidth();
        DrawList->AddLine(l,r,gridColor,thickness);
    }
}

struct GrabHandle {
    ImVec2 pos;    
    bool grabbed = false;
    ImVec2 constrain(float xMin, float xMax, float yMin, float yMax) {       
        ImVec2 constrained = pos;
        pos.x = ImClamp(pos.x,xMin,xMax);
        pos.y = ImClamp(pos.y, yMin, yMax);
        return pos - constrained;
    }
};

struct GrabGroup : std::enable_shared_from_this<GrabGroup> {

    GrabGroup(ImVec2 pos) {
        p.pos = pos;
        l.pos = pos - ImVec2(0.1f,0);
        r.pos = pos + ImVec2(0.1f,0);
    }

    void constrain() {
        if (forceTanget) {
            if (r.grabbed) {
                auto v = r.pos - p.pos;
                l.pos = p.pos - v;
            }
            else {
                auto v = l.pos - p.pos;
                r.pos = p.pos - v;
            }
        } 
        p.constrain(prev ? prev->p.pos.x : 0, next ? (next->p.grabbed ? p.pos.x : next->p.pos.x) : 1, 0, 1);
        l.constrain(prev ? prev->p.pos.x : -FLT_MAX, p.pos.x, -FLT_MAX, FLT_MAX);
        r.constrain(p.pos.x, next ? next->p.pos.x : FLT_MAX, -FLT_MAX, FLT_MAX);   
    }

    ImVec2 constrainP() {
        return p.constrain(prev ? prev->p.pos.x : 0, next ? (next->p.grabbed ? p.pos.x : next->p.pos.x) : 1, 0, 1);
    }

    void insert(std::shared_ptr<GrabGroup> g) {
        if (prev) {
            prev->next = g;
            g->prev = prev;
        }
        g->next = shared_from_this();
        prev = g;
    }

    void push(std::shared_ptr<GrabGroup> g) {
        next = g;
        g->prev = shared_from_this();
    }

    void remove() {
        if (prev)
            prev->next = next;
        if (next)
            next->prev = prev;
    }

    GrabHandle l, p, r;
    bool active = false;
    bool forceTanget = true;

    std::shared_ptr<GrabGroup> prev = nullptr;
    std::shared_ptr<GrabGroup> next = nullptr;
};

struct PolyBezier {
    int pointCount() {
        int points = 0;
        for (auto g = root; g; g = g->next) 
            points++;
        return points;
    }
    std::shared_ptr<GrabGroup> root;
    std::shared_ptr<GrabGroup> head;
};

void PolyBezierEditor(const char* label, PolyBezier& pb, float grabRadius, ImVec4 color, float thickness = 1.0f, int segments = 64, ImVec2 size = ImVec2(-1,0)) {
    // skip render if SkipItems on
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;
    // get ImGuiContext and Style
    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImGuiIO& IO = GetIO();
    ImDrawList* DrawList = GetWindowDrawList();
    // convert color
    auto color32 = ColorConvertFloat4ToU32(color);
    // sizing
    size = CalcItemSize(size, 200, 200);
    const ImRect frame_bb(window->DC.CursorPos, window->DC.CursorPos + size);
    const ImVec2 padding(10,10);
    const ImRect inner_bb(frame_bb.Min + padding, frame_bb.Max - padding);
    const float bb_width = inner_bb.GetWidth();
    const float bb_height = inner_bb.GetHeight();
    const float bb_invWidth = 1.0f / inner_bb.GetWidth();
    const float bb_invHeight = 1.0f / inner_bb.GetHeight();
    // render background
    ItemSize(frame_bb, style.FramePadding.y);
    if (!ItemAdd(frame_bb, 0, &frame_bb))
        return;
    const bool hovered = ItemHoverable(inner_bb, id);
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);
    RenderGrid(inner_bb, 10, 10, GetColorU32(ImGuiCol_WindowBg), GetColorU32(ImGuiCol_FrameBgHovered, 0.05f));
    // clear active lambda
    auto clearActivations = [&](std::shared_ptr<GrabGroup> except = nullptr) {
        for (auto g = pb.root; g; g = g->next){
            if (g != except)
                g->active = false;
        }
    };
    // conversion lambdas
    auto toPx = [&](const ImVec2& nm) -> ImVec2 {
        auto px = ImVec2(inner_bb.Min.x + nm.x * bb_width, inner_bb.Max.y - nm.y * bb_height);
        return px;
    };
    auto toNm = [&](const ImVec2& px) -> ImVec2 {
        auto nm = ImVec2((px.x-inner_bb.Min.x) * bb_invWidth, -(px.y-inner_bb.Max.y) * bb_invHeight);
        return nm;
    };
    auto toNmD = [&](const ImVec2& px) -> ImVec2 {
        auto nmD = ImVec2(px.x * bb_invWidth, - px.y * bb_invHeight);
        return nmD;
    };    
    // check for additions
    if (IO.MouseClicked[1] && hovered) {
        auto newG = std::make_shared<GrabGroup>(toNm(IO.MousePos));
        if (!pb.root) {
            pb.root = newG;
            pb.head = pb.root;
        }
        else
        {
            bool inserted = false;
            for (auto g = pb.root; g; g = g->next)
            {
                if (g->p.pos.x > newG->p.pos.x) {
                    g->insert(newG);
                    if (g == pb.root)
                        pb.root = newG;
                    inserted = true;
                    break;
                }
            }
            if (!inserted) {
                pb.head->push(newG);
                pb.head = newG;
            }
        }  
        clearActivations();
        newG->active = true;
        newG->p.grabbed = true;
    }
    // check for deletions
    if (ImGui::IsKeyPressedMap(ImGuiKey_Delete)) {
        for (auto g = pb.root; g; g = g->next) {
            if (g->active && g != pb.root && g != pb.head) {
                g->remove();
            }
        }
    }
    // drag 
    bool clearActive = true;
    for (auto g = pb.root; g; g = g->next) {
        ImVec2 v;
        float d;
        if (g->active) {
            // drag left cp
            v = (IO.MousePos - toPx(g->l.pos));
            d = v.x * v.x + v.y * v.y;  
            if (d < 25) {
                if (IO.MouseClicked[0]) {
                    if (IO.KeyCtrl)
                        g->forceTanget = !g->forceTanget;
                    g->l.grabbed = true;
                    clearActive = false;
                }   
            }
            if (g->l.grabbed) {
                g->l.pos += toNmD(IO.MouseDelta);
                if (IO.MouseReleased[0]) 
                    g->l.grabbed = false;
                break;
            }
            // drag right cp
            v = (IO.MousePos - toPx(g->r.pos));
            d = v.x * v.x + v.y * v.y;  
            if (d < 25) {
                if (IO.MouseClicked[0]) {
                    if (IO.KeyCtrl)
                        g->forceTanget = !g->forceTanget;
                    g->r.grabbed = true;
                    clearActive = false;
                }   
            }
            if (g->r.grabbed) {
                g->r.pos += toNmD(IO.MouseDelta);
                if (IO.MouseReleased[0]) 
                    g->r.grabbed = false;
                break;
            }
        } 
        // drag point
        v = (IO.MousePos - toPx(g->p.pos));
        d = v.x * v.x + v.y * v.y;
        if (d < 25) {
            if (IO.MouseClicked[0]) {
                if (IO.KeyCtrl)
                    g->forceTanget = !g->forceTanget;
                g->p.grabbed = true;
                g->active = true;
                clearActive = false; 
                clearActivations(g);                
            }
        }
        if (g->p.grabbed) {
            g->p.pos += toNmD(IO.MouseDelta);
            auto constrainedBy = g->constrainP();
            g->r.pos += toNmD(IO.MouseDelta) + constrainedBy;
            g->l.pos += toNmD(IO.MouseDelta) + constrainedBy;
            if (IO.MouseReleased[0] || IO.MouseReleased[1]) 
                g->p.grabbed = false;
            break;
        }
    }
    // constrain
    for (auto g = pb.root; g; g = g->next) 
        g->constrain();
    // clear active if clicked
    if (IO.MouseClicked[0] && clearActive)
        clearActivations();
    // check if anything active
    bool active = false;
    for (auto g = pb.root; g; g = g->next)
    {
        if (g->active) {
            active = true;
            break;
        }
    } 
    // count points
    int points = pb.pointCount();
    // render points
    if (hovered || active) {
        for (auto g = pb.root; g; g = g->next) 
            DrawList->AddCircleFilled(toPx(g->p.pos), grabRadius, color32);
    }
    // render bezier
    if (points > 0) {
        DrawList->PushClipRect(inner_bb.Min, inner_bb.Max, true);
        auto g = pb.root;
        for (int i = 0; i < points-1; ++i) {
            DrawList->AddBezierCurve(toPx(g->p.pos), toPx(g->r.pos), toPx(g->next->l.pos), toPx(g->next->p.pos), color32, thickness, segments);
            g = g->next;
        }
        DrawList->PopClipRect();
    }
    // render active grabs
    for (auto g = pb.root; g; g = g->next) {
        if (g->active) {
            int segs = 12;
            if (g->prev) {
                    ImGui::GetForegroundDrawList()->AddLine(toPx(g->l.pos), toPx(g->p.pos), GetColorU32(ImGuiCol_ButtonActive));

                if (g->forceTanget)
                    ImGui::GetForegroundDrawList()->AddRectFilled(toPx(g->l.pos) - ImVec2(grabRadius,grabRadius), toPx(g->l.pos) + ImVec2(grabRadius,grabRadius), GetColorU32(ImGuiCol_ButtonActive));
                else 
                    ImGui::GetForegroundDrawList()->AddRect(toPx(g->l.pos) - ImVec2(grabRadius,grabRadius), toPx(g->l.pos) + ImVec2(grabRadius,grabRadius), GetColorU32(ImGuiCol_ButtonActive));
                
            }
            if (g->next) {
                ImGui::GetForegroundDrawList()->AddLine(toPx(g->r.pos), toPx(g->p.pos), GetColorU32(ImGuiCol_ButtonActive));
                if (g->forceTanget)
                    ImGui::GetForegroundDrawList()->AddRectFilled(toPx(g->r.pos) - ImVec2(grabRadius,grabRadius), toPx(g->r.pos) + ImVec2(grabRadius,grabRadius), GetColorU32(ImGuiCol_ButtonActive));                
                else 
                    ImGui::GetForegroundDrawList()->AddRect(toPx(g->r.pos) - ImVec2(grabRadius,grabRadius), toPx(g->r.pos) + ImVec2(grabRadius,grabRadius), GetColorU32(ImGuiCol_ButtonActive));                
            }
        }
    }
}

};

class Window : public GameObject {
public:

    ImGui::PolyBezier pb1, pb2;

    void start() {
        // m_root = std::make_shared<ImGui::GrabGroup>(ImVec2(50,50));
        // m_root->next = std::make_shared<ImGui::GrabGroup>(ImVec2(300,150));
    }

    void update() {
        auto winSize = static_cast<Vector2f>(Engine::window->getSize());
        ImGui::SetNextWindowPos(Vector2f(5,5), ImGuiCond_Always);
        ImGui::SetNextWindowSize(Vector2f(winSize.x - 10, winSize.y - 10), ImGuiCond_Always);
        ImGui::Begin("Widgets", &visible, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
        ImGui::PolyBezierEditor("PolyBezier1", pb1, 2.5, Blues::DeepSkyBlue);
        ImGui::End();
    }


    bool visible;
};

int main() {
    Engine::init(400,400, WindowStyle::Default, "Widgets");
    Engine::makeRoot<Window>();
    Engine::run();
}