#include <carnot>
#include <map>
#include <sstream>

using namespace carnot;

using std::vector;
using std::map;
using std::string;
using std::ostringstream;

class UdpClient : public GameObject {
public:

    UdpClient(unsigned short localPort, unsigned short remotePort, IpAddress remoteAddress) :
        m_remoteAddress(remoteAddress),
        m_localPort(localPort),
        m_remotePort(remotePort),
        m_sent(0),
        m_recv(0),
        m_told(0)
    {
        m_udp.bind(m_localPort);
        m_udp.setBlocking(false);

        m_textRend = addComponent<TextRenderer>();

        m_textRend->text.setCharacterSize(15);
        m_textRend->text.setString("Client");
        alignBottomCenter(m_textRend->text);

        m_sendRend = addComponent<ShapeRenderer>(make<SquareShape>(15));
        m_recvRend = addComponent<ShapeRenderer>(make<SquareShape>(15));
        m_sendRend->setColor(Grays::Gray20);
        m_recvRend->setColor(Grays::Gray20);

        m_sendRend->getShape()->move(-15, 20);
        m_recvRend->getShape()->move(15, 20);
    }

    void start() override {

    }

    void update() override {
        receive();
        if (Input::getKeyDown(Key::Space))
            send();
        if (Input::getKeyDown(Key::S))
            startCoroutine(send(100));
    }

    void send() {
        m_sent++;
        Packet packet;
        packet << m_sent;
        m_udp.send(packet, m_remoteAddress, m_remotePort);
        m_sendClock.restart();
    }

    Enumerator send(int n) {
        for (int i = 0; i < n; ++i) {
            send();
            co_yield nullptr;
        }
    }

    void receive() {
        Packet packet;
        IpAddress remoteAddress;
        unsigned short remotePort;
        while (m_udp.receive(packet, remoteAddress, remotePort) == Socket::Done) {
            packet >> m_told;
            m_recv++;
            m_recvClock.restart();
            m_remoteAddress = remoteAddress;
            m_remotePort = remotePort;
        }
    }

    void lateUpdate() override {
        ostringstream ss;
        ss << "Sent: " << m_sent << std::endl;
        ss << "Recv: " << m_recv << std::endl;
        ss << "Told: " << m_told << std::endl;

        m_textRend->text.setString(ss.str());
        alignBottomCenter(m_textRend->text);

        float t;
        t = Math::clamp01(m_sendClock.getElapsedTime().asSeconds() / 0.1f);
        m_sendRend->setColor(Tween::Smoothstep(Greens::Chartreuse, Grays::Gray20, t));
        t = Math::clamp01(m_recvClock.getElapsedTime().asSeconds() / 0.1f);
        m_recvRend->setColor(Tween::Smoothstep(Yellows::Yellow, Grays::Gray20, t));
    }

private:
    IpAddress m_remoteAddress;
    unsigned short m_localPort;
    unsigned short m_remotePort;
    UdpSocket m_udp;

    int m_sent;
    int m_recv;
    int m_told;

    Ptr<SquareShape> m_sendLight;
    Ptr<SquareShape> m_recvLight;

    Handle<ShapeRenderer> m_sendRend;
    Handle<ShapeRenderer> m_recvRend;

    Handle<TextRenderer> m_textRend;

    Clock m_sendClock;
    Clock m_recvClock;
};

int main(int argc, char *argv[])
{
    Options options("online.exe", "Online Example");
    options.add_options()
        ("l,local",  "Local Port",  value<int>())
        ("r,remote", "Remote Port", value<int>())
        ("i,ip",     "Remote IP  ", value<string>());
    auto input = options.parse(argc, argv);
    if (input.count("l") == 0 || input.count("r") == 0 || input.count("i") == 0) {
        print("Invalid arguments provided. Example usaged:");
        print("computer-a> online.exe -l 55001 -r 55002 -i 192.168.1.2");
        print("computer-b> online.exe -l 55002 -l 55001 -i 192.168.1.1");
        return 1;
    }

    Engine::init(250,250,"Online Example");
    Engine::makeRoot<UdpClient>(input["l"].as<int>(),input["r"].as<int>(),input["i"].as<string>())->transform.setPosition(125, 125);
    Engine::run();
    return 0;
}
