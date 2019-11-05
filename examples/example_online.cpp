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

//=============================================================================
// AVATAR
//=============================================================================

class Avatar : public GameObject {
public:
    Avatar(const string& username, const Color& color) :
        m_username(username)
    {
        m_sr = addComponent<ShapeRenderer>(make<SquareShape>(50));
        m_sr->setColor(color);

        m_text = addComponent<TextRenderer>();
        m_text->text.setCharacterSize(20);
        m_text->text.setString(m_username);
        m_text->text.setPosition(-25, -55);
        transform.setPosition(Random::range(100.0f, 900.0f), Random::range(100.0f, 900.0f));
    }

    void start() override {

    }

    const string m_username;
    Handle<TextRenderer> m_text;
    Handle<ShapeRenderer> m_sr;
};

//=============================================================================
// PLAYER
//=============================================================================

class Player : public Avatar {
public:
    Player(const string& username) : Avatar(username, Random::color()) {}
    void update() override {
        if (Input::getKey(Key::Up))
            transform.move(0, -100 * Engine::deltaTime());
        if (Input::getKey(Key::Down))
            transform.move(0, 100 * Engine::deltaTime());
        if (Input::getKey(Key::Left))
            transform.move(-100 * Engine::deltaTime(), 0);
        if (Input::getKey(Key::Right))
            transform.move(100 * Engine::deltaTime(), 0);
    }
};

//=============================================================================
// NETWORK OBJECT
//=============================================================================

class NetworkObject : public GameObject {
public:
    NetworkObject(const string& username) :
        m_connected(false)
    {
        m_localAddress = IpAddress::getLocalAddress().toString();
        m_publicAddress = IpAddress::getPublicAddress().toString();
        m_status = "Disconnected";

        m_text = addComponent<TextRenderer>();
        m_text->text.setCharacterSize(20);
        m_text->text.setPosition(5, 5);

        m_notifyText = addComponent<TextRenderer>();
        m_notifyText->text = m_text->text;
        m_notifyText->text.setPosition(5, 970);

        m_player = makeChild<Player>(username);
        notify("Welcome!");
    }

    void update() {
        updateText();
    }

    void updateText() {
        ostringstream ss;
        ss << "Status:    " << m_status << std::endl;
        ss << "Local IP:  " << m_localAddress << std::endl;
        ss << "Public IP: " << m_publicAddress << std::endl;
        m_text->text.setString(ss.str());
        float t = Math::clamp01(m_notifyClock.getElapsedTime().asSeconds() / 3.0f);
        Color c = Tween::Quadratic::In(Color::White, Color::Transparent, t);
        m_notifyText->text.setFillColor(c);
    }

    void notify(const string& message) {
        m_notifyClock.restart();
        m_notifyText->text.setString(message);
    }

    void addAvatar(Packet& addPacket) {
        string username;
        float x, y;
        int color;
        addPacket >> username >> color >> x >> y;
        m_others[username] = makeChild<Avatar>(username, Color(color));
        m_others[username]->transform.setPosition(x, y);
        notify("Connected " + username);
    }

    void destroyAvatar(Packet& destroyPacket) {

    }

    void updateAvatars(Packet& updatePacket) {
        int n;
        updatePacket >> n;
        for (int i = 0; i < n; ++i) {
            string username;
            float x, y;
            updatePacket >> username >> x >> y;
            if (m_others.count(username))
                m_others[username]->transform.setPosition(x, y);
        }
    }

    Handle<Player> m_player;
    map<string, Handle<Avatar>> m_others;
    string m_localAddress;
    string m_publicAddress;
    string m_status;
    Handle<TextRenderer> m_text;
    Handle<TextRenderer> m_notifyText;
    bool m_connected;
    Clock m_notifyClock;
};

//=============================================================================
// CLIENT
//=============================================================================

class Client : public NetworkObject {
public:
    Client(const string& username) :
        NetworkObject(username)
    {
    }

    void update() override {
        if (Input::getKeyDown(Key::C) && !m_connected)
            startCoroutine(connect());
        Packet packet;
        if (m_connected) {
            auto position = m_player->transform.getPosition();
            packet << m_player->m_username << position.x << position.y;
            m_tcp.send(packet);
            if (m_tcp.receive(packet) == Socket::Disconnected) {
                m_status = "Disconnected";
                notify("Lost connectin to Server");
                m_tcp.setBlocking(true);
                m_connected = false;
            }
        }
    }

    Enumerator findServer(IpAddress& serverAddress) {
        m_status = "Searching";
        UdpSocket udp;
        udp.bind(CLIENT_UDP);
        unsigned short serverPort;
        Packet packet;
        udp.setBlocking(false);
        while (udp.receive(packet, serverAddress, serverPort) != Socket::Status::Done) {
            co_yield nullptr;
        }
        m_status = "Found";
    }

    Enumerator tryConnect(IpAddress serverAddress) {
        m_status = "Connecting";
        while (m_tcp.connect(serverAddress, SERVER_TCP, milliseconds(1)) != Socket::Status::Done) {
            co_yield nullptr;
        }
        auto position = m_player->transform.getPosition();
        Packet packet;
        packet << m_player->m_username
               << m_player->m_sr->getColor().toInteger()
               << position.x << position.y;
        m_tcp.send(packet);
        co_yield new WaitForSeconds(1.0f);
        string response;
        m_tcp.receive(packet);
        packet >> response;
        if (response != "OK")
            m_status = "Rejected: " + response;
        else
            m_status = "Connected";
    }

    Enumerator connect() {
        IpAddress serverAddress;
        co_yield startCoroutine(findServer(serverAddress));
        co_yield startCoroutine(tryConnect(serverAddress));
        m_connected = true;
        m_tcp.setBlocking(false);
    }

    TcpSocket m_tcp;
};

//=============================================================================
// SERVER
//=============================================================================

class Server : public NetworkObject {
public:
    Server(const std::string& username) :
        NetworkObject(username)
    {
        if (m_udp.bind(SERVER_UDP) == Socket::Error)
            print(m_status = "UDP Failed");
        if (m_listener.listen(SERVER_TCP) == Socket::Error)
            print(m_status = "TCP Failed");
        m_selector.add(m_listener);

    }

    ~Server() {
        for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
            TcpSocket* client = it->second;
            m_selector.remove(*client);
            client->disconnect();
            delete client;
        }
    }

    void update() override {
        pollSocket();
        broadcastAddress();
    }

    void pollSocket() {
        if (m_selector.wait(milliseconds(1))) {
            // check for new connections
            if (m_selector.isReady(m_listener))
                connectClient();
            else {
                vector<Packet> packets;
                vector<string> disconnections;
                for (auto it = m_clients.begin(); it != m_clients.end(); ++it) {
                    TcpSocket& client = *(it->second);
                    Packet clientPacket;
                    bool packetReceived = false;
                    if (m_selector.isReady(client)) {
                        Packet tempPacket;
                        if (client.receive(tempPacket) == Socket::Disconnected) {
                            disconnections.push_back(it->first);
                            packetReceived = false;
                            break;
                        }
                        else {
                            packetReceived = true;
                            clientPacket = tempPacket;
                            tempPacket.clear();
                            while (client.receive(tempPacket) == Socket::Done) {
                                clientPacket = tempPacket;
                                tempPacket.clear();
                            }
                        }
                    }
                    if (packetReceived)
                        packets.push_back(clientPacket);

                }
                processDisconnections(disconnections);
                processPackets(packets);
            }
        }
    }

    void broadcastAddress() {
        Packet empty;
        m_udp.send(empty, IpAddress::Broadcast, CLIENT_UDP);
    }

    void processDisconnections(const vector<string>& disconnections) {
        for (auto& d : disconnections)
            disconnectClient(d);
    }

    void processPackets(vector<Packet>& packets) {
        Packet updatePacket;
        int n = (int)packets.size();
        string comm = "u";
        updatePacket << comm << n;
        for (auto& p : packets) {
            string username;
            float x, y;
            p >> username >> x >> y;
            updatePacket << username << x << y;
        }
        updatePacket >> comm;
        updateAvatars(updatePacket);
    }

    void connectClient() {
        TcpSocket* client = new TcpSocket;
        if (m_listener.accept(*client) == Socket::Done) {
            Packet packet;
            string username;
            sf::Uint32 color;
            float x, y;
            client->receive(packet);
            packet >> username >> color >> x >> y;
            packet.clear();
            if (m_clients.count(username)) {
                packet << ("Username " + username + " is unavailable");
                client->send(packet);
                delete client;
            }
            else {
                client->setBlocking(false);
                m_clients[username] = client;
                m_selector.add(*client);
                packet << "OK";
                client->send(packet);
                packet.clear();
                string comm = "a";
                packet << comm << username << color << x << y;
                packet >> comm; // pop command for self
                addAvatar(packet);
            }
        }
        else
            delete client;
    }

    void disconnectClient(const string& username) {
        m_selector.remove(*m_clients[username]);
        delete m_clients[username];
        m_clients.erase(username);
        m_others[username]->destroy();
        m_others.erase(username);
        notify("Disconnected " + username);
    }

    UdpSocket m_udp;
    TcpListener m_listener;
    SocketSelector m_selector;
    map<string, TcpSocket*> m_clients;
};

//=============================================================================
// MAIN
//=============================================================================

int main(int argc, char *argv[])
{
    Options options("online.exe", "Online Example");
    options.add_options()
        ("s,server","Makes this computer the Server")
        ("u,user",  "Set username",            value<string>())
        ("i,ip",    "IP dddress of the Sever", value<string>());
    auto input = options.parse(argc, argv);

    if (input.count("user")) {
        Engine::init(1000,1000,"Online Demo");
        if (input.count("server")) {
            Engine::makeRoot<Server>(input["u"].as<string>());
        }
        else {
            Engine::makeRoot<Client>(input["u"].as<string>());
        }
        Engine::run();
    }
    else {
        print(options.help());
        print("  Examples:");
        print("    Server:       ./online.exe -u epezent -s");
        print("    Client (LAN): ./online.exe -u jpezent");
        print("    Client (www): ./online.exe -u mpezent -i 68.225.108.149\n");
    }

    return 0;
}