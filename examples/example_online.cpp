#include <carnot>
#include <map>
#include <sstream>

using namespace carnot;

using std::vector;
using std::map;
using std::string;
using std::ostringstream;

const unsigned short SERVER_TCP = 55001; // port 55001 (server TCP port) must be
const unsigned short SERVER_UDP = 55002; // port-forwarded on the server machine
const unsigned short CLIENT_UDP = 55003; // to enable internet-based chat

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

    Engine::init(250,250,"Online Example");
    Engine::makeRoot<UdpClient>(input["l"].as<int>(),input["r"].as<int>(),input["i"].as<string>())->transform.setPosition(125, 125);
    Engine::run();
    return 0;
}


//=============================================================================
// // AVATAR
// //=============================================================================

// class Avatar : public Object {
// public:
//     Avatar(const string& username, const Color& color) :
//         m_username(username),
//         m_sqr(50)
//     {
//         m_sqr.setColor(color);
//         m_text.setFont(engine().fonts.get("RobotoMonoBold"));
//         m_text.setCharacterSize(20);
//         m_text.setFillColor(Color::White);
//         m_text.setString(m_username);
//         transform.setPosition(random(100.0f, 900.0f), random(100.0f, 900.0f));
//         m_text.setPosition(-25, -55);
//     }

//     void start() override {

//     }

//     void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
//         target.draw(m_sqr, states);
//         target.draw(m_text, states);
//     }

//     const string m_username;
//     SquareShape m_sqr;
//     Text m_text;
// };

// //=============================================================================
// // PLAYER
// //=============================================================================

// class Player : public Avatar {
// public:
//     Player(const string& username) : Avatar(username, randomColor()) {}
//     void update() override {
//         if (Input::getKey(Key::Up))
//             transform.move(0, -100 * Engine::deltaTime());
//         if (Input::getKey(Key::Down))
//             transform.move(0, 100 * Engine::deltaTime());
//         if (Input::getKey(Key::Left))
//             transform.move(-100 * Engine::deltaTime(), 0);
//         if (Input::getKey(Key::Right))
//             transform.move(100 * Engine::deltaTime(), 0);
//     }
// };

// //=============================================================================
// // NETWORK OBJECT
// //=============================================================================

// class NetworkObject : public Object {
// public:
//     NetworkObject(const string& username) :
//         m_connected(false)
//     {
//         m_localAddress = IpAddress::getLocalAddress().toString();
//         m_publicAddress = IpAddress::getPublicAddress().toString();
//         m_status = "Disconnected";
//         m_text.setFont(engine().fonts.get("RobotoMonoBold"));
//         m_text.setCharacterSize(20);
//         m_text.setFillColor(Color::White);
//         m_text.setPosition(5, 5);
//         m_notifyText = m_text;
//         m_notifyText.setPosition(5, 970);
//         m_player = makeChild<Player>(username);
//         notify("Welcome!");
//     }

//     void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
//         updateText();
//         target.draw(m_text, states);
//         target.draw(m_notifyText, states);
//     }

//     void updateText() const {
//         ostringstream ss;
//         ss << "Status:    " << m_status << std::endl;
//         ss << "Local IP:  " << m_localAddress << std::endl;
//         ss << "Public IP: " << m_publicAddress << std::endl;
//         m_text.setString(ss.str());
//         float t = clamp01(m_notifyClock.getElapsedTime().asSeconds() / 3.0f);
//         Color c = Tween::Quadratic::In(Color::White, Color::Transparent, t);
//         m_notifyText.setFillColor(c);
//     }

//     void notify(const string& message) {
//         m_notifyClock.restart();
//         m_notifyText.setString(message);
//     }

//     void addAvatar(Packet& addPacket) {
//         string username;
//         float x, y;
//         int color;
//         addPacket >> username >> color >> x >> y;
//         m_others[username] = makeChild<Avatar>(username, Color(color));
//         m_others[username]->transform.setPosition(x, y);
//         notify("Connected " + username);
//     }

//     void destroyAvatar(Packet& destroyPacket) {

//     }

//     void updateAvatars(Packet& updatePacket) {
//         int n;
//         updatePacket >> n;
//         for (int i = 0; i < n; ++i) {
//             string username;
//             float x, y;
//             updatePacket >> username >> x >> y;
//             if (m_others.count(username))
//                 m_others[username]->transform.setPosition(x, y);
//         }
//     }

//     Handle<Player> m_player;
//     map<string, Handle<Avatar>> m_others;
//     string m_localAddress;
//     string m_publicAddress;
//     string m_status;
//     mutable Text m_text;
//     mutable Text m_notifyText;
//     bool m_connected;
//     Clock m_notifyClock;
// };

// //=============================================================================
// // CLIENT
// //=============================================================================

// class Client : public NetworkObject {
// public:
//     Client(const string& username) :
//         NetworkObject(username)
//     {
//     }

//     void update() override {
//         if (Input::getKeyDown(Key::C) && !m_connected)
//             startCoroutine(connect());
//         Packet packet;
//         if (m_connected) {
//             auto position = m_player->transform.getPosition();
//             packet << m_player->m_username << position.x << position.y;
//             m_tcp.send(packet);
//             if (m_tcp.receive(packet) == Socket::Disconnected) {
//                 m_status = "Disconnected";
//                 notify("Lost connectin to Server");
//                 m_tcp.setBlocking(true);
//                 m_connected = false;
//             }
//         }
//     }

//     Enumerator findServer(IpAddress& serverAddress) {
//         m_status = "Searching";
//         UdpSocket udp;
//         udp.bind(CLIENT_UDP);
//         unsigned short serverPort;
//         Packet packet;
//         udp.setBlocking(false);
//         while (udp.receive(packet, serverAddress, serverPort) != Socket::Status::Done) {
//             co_yield nullptr;
//         }
//         m_status = "Found";
//     }

//     Enumerator tryConnect(IpAddress serverAddress) {
//         m_status = "Connecting";
//         while (m_tcp.connect(serverAddress, SERVER_TCP, milliseconds(1)) != Socket::Status::Done) {
//             co_yield nullptr;
//         }
//         auto position = m_player->transform.getPosition();
//         Packet packet;
//         packet << m_player->m_username
//                << m_player->m_sqr.getColor().toInteger()
//                << position.x << position.y;
//         m_tcp.send(packet);
//         co_yield new WaitForSeconds(1.0f);
//         string response;
//         m_tcp.receive(packet);
//         packet >> response;
//         if (response != "OK")
//             m_status = "Rejected: " + response;
//         else
//             m_status = "Connected";
//     }

//     Enumerator connect() {
//         IpAddress serverAddress;
//         co_yield startCoroutine(findServer(serverAddress));
//         co_yield startCoroutine(tryConnect(serverAddress));
//         m_connected = true;
//         m_tcp.setBlocking(false);
//     }

//     TcpSocket m_tcp;
// };

// //=============================================================================
// // SERVER
// //=============================================================================

// class Server : public NetworkObject {
// public:
//     Server(const std::string& username) :
//         NetworkObject(username)
//     {
//         if (m_udp.bind(SERVER_UDP) == Socket::Error)
//             print(m_status = "UDP Failed");
//         if (m_listener.listen(SERVER_TCP) == Socket::Error)
//             print(m_status = "TCP Failed");
//         m_selector.add(m_listener);

//     }

//     ~Server() {
//         for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
//             TcpSocket* client = it->second;
//             m_selector.remove(*client);
//             client->disconnect();
//             delete client;
//         }
//     }

//     void update() override {
//         pollSocket();
//         broadcastAddress();
//     }

//     void pollSocket() {
//         if (m_selector.wait(milliseconds(1))) {
//             // check for new connections
//             if (m_selector.isReady(m_listener))
//                 connectClient();
//             else {
//                 vector<Packet> packets;
//                 vector<string> disconnections;
//                 for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
//                     TcpSocket& client = *(it->second);
//                     Packet clientPacket;
//                     bool packetReceived = false;
//                     if (m_selector.isReady(client)) {
//                         Packet tempPacket;
//                         if (client.receive(tempPacket) == Socket::Disconnected) {
//                             disconnections.push_back(it->first);
//                             packetReceived = false;
//                             break;
//                         }
//                         else {
//                             packetReceived = true;
//                             clientPacket = tempPacket;
//                             tempPacket.clear();
//                             while (client.receive(tempPacket) == Socket::Done) {
//                                 clientPacket = tempPacket;
//                                 tempPacket.clear();
//                             }
//                         }
//                     }
//                     if (packetReceived)
//                         packets.push_back(clientPacket);

//                 }
//                 processDisconnections(disconnections);
//                 processPackets(packets);
//             }
//         }
//     }

//     void broadcastAddress() {
//         Packet empty;
//         m_udp.send(empty, IpAddress::Broadcast, CLIENT_UDP);
//     }

//     void processDisconnections(const vector<string>& disconnections) {
//         for (auto& d : disconnections)
//             disconnectClient(d);
//     }

//     void processPackets(vector<Packet>& packets) {
//         Packet updatePacket;
//         int n = (int)packets.size();
//         string comm = "u";
//         updatePacket << comm << n;
//         for (auto& p : packets) {
//             string username;
//             float x, y;
//             p >> username >> x >> y;
//             updatePacket << username << x << y;
//         }
//         updatePacket >> comm;
//         updateAvatars(updatePacket);
//     }

//     void connectClient() {
//         TcpSocket* client = new TcpSocket;
//         if (m_listener.accept(*client) == Socket::Done) {
//             Packet packet;
//             string username;
//             sf::Uint32 color;
//             float x, y;
//             client->receive(packet);
//             packet >> username >> color >> x >> y;
//             packet.clear();
//             if (m_clients.count(username)) {
//                 packet << ("Username " + username + " is unavailable");
//                 client->send(packet);
//                 delete client;
//             }
//             else {
//                 client->setBlocking(false);
//                 m_clients[username] = client;
//                 m_selector.add(*client);
//                 packet << "OK";
//                 client->send(packet);
//                 packet.clear();
//                 string comm = "a";
//                 packet << comm << username << color << x << y;
//                 packet >> comm; // pop command for self
//                 addAvatar(packet);
//             }
//         }
//         else
//             delete client;
//     }

//     void disconnectClient(const string& username) {
//         m_selector.remove(*m_clients[username]);
//         delete m_clients[username];
//         m_clients.erase(username);
//         m_others[username]->destroy();
//         m_others.erase(username);
//         notify("Disconnected " + username);
//     }

//     UdpSocket m_udp;
//     TcpListener m_listener;
//     SocketSelector m_selector;
//     map<string, TcpSocket*> m_clients;
// };

// //=============================================================================
// // MAIN
// //=============================================================================

// int main2(int argc, char *argv[])
// {
//     Options options("online.exe", "Online Example");
//     options.add_options()
//         ("s,server","Makes this computer the Server")
//         ("u,user",  "Set username",            value<string>())
//         ("i,ip",    "IP dddress of the Sever", value<string>());
//     auto input = options.parse(argc, argv);

//     if (input.count("user")) {
//         // g_font.loadFromFile("RobotoMono-Bold.ttf");
//         Engine engine(1000,1000);
//         engine.window.setTitle("Online Example");
//         if (input.count("server")) {
//             engine.makeRoot<Server>(input["u"].as<string>());
//         }
//         else {
//             engine.makeRoot<Client>(input["u"].as<string>());
//         }
//         engine.run();
//     }
//     else {
//         print(options.help());
//         print("  Examples:");
//         print("    Server:       ./online.exe -u epezent -s");
//         print("    Client (LAN): ./online.exe -u jpezent");
//         print("    Client (www): ./online.exe -u mpezent -i 68.225.108.149\n");
//     }

//     return 0;
// }