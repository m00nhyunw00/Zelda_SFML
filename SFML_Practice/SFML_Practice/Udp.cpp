#include <SFML/Network.hpp>

#include <atomic> //여러 스레드가 동시에 접근하는 값을 안전하게 다루기 위한 타입
#include <iostream>
#include <optional> //값이 있을 수도 있고 없을 수도 있음을 표현하는 타입
#include <string>
#include <thread>

namespace
{
    // 한 번에 받을 수 있는 최대 메시지 크기입니다.
    // 여기서는 1024바이트까지 받도록 단순하게 제한합니다.
    constexpr std::size_t kMaxMessageSize = 1024;
}

int main(int argc, char* argv[])
{
    // 실행 인자는 총 4개여야 합니다.
    // argv[0] : 실행 파일 이름
    // argv[1] : 내가 수신할 포트 번호
    // argv[2] : 상대방 IP 주소
    // argv[3] : 상대방 수신 포트 번호
    if (argc != 4)
    {
        std::cout << "Usage: udp_chat <localPort> <remoteIp> <remotePort>\n";
        return 1;
    }

    // 문자열로 들어온 포트 번호를 unsigned short 타입으로 변환합니다.
    // localPort는 내가 이 프로그램에서 받을 포트입니다.
    const unsigned short localPort = static_cast<unsigned short>(std::stoi(argv[1]));

    // remotePort는 상대 프로그램이 받고 있는 포트입니다.
    const unsigned short remotePort = static_cast<unsigned short>(std::stoi(argv[3]));

    // 문자열 IP를 sf::IpAddress로 변환합니다.
    // 변환에 실패할 수 있으므로 optional로 받습니다.
    const std::optional<sf::IpAddress> remoteIp = sf::IpAddress::fromString(argv[2]);

    // IP 문자열이 잘못되었으면 프로그램을 종료합니다.
    if (!remoteIp)
    {
        std::cerr << "Invalid remote IP address: " << argv[2] << "\n";
        return 1;
    }

    // UDP 통신에 사용할 소켓 객체를 생성합니다.
    sf::UdpSocket socket;

    // 내가 받을 포트(localPort)에 소켓을 바인딩합니다.
    // 바인딩해야 해당 포트로 들어오는 UDP 데이터를 받을 수 있습니다.
    if (socket.bind(localPort) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to bind UDP socket to port " << localPort << "\n";
        return 1;
    }

    // 논블로킹 모드로 설정합니다.
    // receive()를 호출했을 때 데이터가 없어도 멈추지 않고 바로 반환됩니다.
    socket.setBlocking(false);

    // 메인 스레드와 수신 스레드가 함께 종료 상태를 공유하기 위한 플래그입니다.
    // atomic을 사용해서 스레드 간 안전하게 값을 읽고 쓸 수 있게 합니다.
    std::atomic<bool> running = true;

    // 수신 전용 스레드입니다.
    // 사용자가 입력하는 동안에도 동시에 네트워크 메시지를 받을 수 있게 만듭니다.
    std::thread receiver([&]()
        {
            // running이 true인 동안 계속 반복하면서 수신을 시도합니다.
            while (running)
            {
                // 수신 데이터를 저장할 버퍼입니다.
                // 매 반복마다 초기화해서 이전 데이터가 남지 않게 합니다.
                char buffer[kMaxMessageSize] = {};

                // 실제로 몇 바이트를 받았는지 저장할 변수입니다.
                std::size_t received = 0;

                // 메시지를 보낸 상대의 IP 주소를 저장합니다.
                // UDP는 연결 기반이 아니므로, 매번 누가 보냈는지 같이 받아야 합니다.
                std::optional<sf::IpAddress> sender;

                // 메시지를 보낸 상대의 포트 번호입니다.
                unsigned short senderPort = 0;

                // UDP 데이터 수신 시도
                // 논블로킹 모드이므로 데이터가 없으면 NotReady를 바로 반환합니다.
                const sf::Socket::Status status =
                    socket.receive(buffer, sizeof(buffer), received, sender, senderPort);

                // 정상적으로 데이터를 받았고, 보낸 사람 정보도 있다면 출력합니다.
                if (status == sf::Socket::Status::Done && sender)
                {
                    std::cout << "\n[" << sender->toString() << ":" << senderPort << "] "
                        << std::string(buffer, received) << "\n> " << std::flush;
                }
                // 데이터가 없는 상태(NotReady)는 정상 상황이므로 에러로 처리하지 않습니다.
                // 그 외 상태는 수신 실패로 보고 메시지를 출력합니다.
                else if (status != sf::Socket::Status::NotReady)
                {
                    std::cerr << "\nReceive error.\n> " << std::flush;
                }

                // 너무 빠르게 무한 반복하면 CPU를 과하게 사용하므로
                // 10ms 정도 잠깐 쉬었다가 다시 수신을 확인합니다.
                sf::sleep(sf::milliseconds(10));
            }
        });

    // 현재 채팅 설정 정보를 사용자에게 출력합니다.
    std::cout << "UDP chat started. Type /quit to exit.\n";
    std::cout << "Local port: " << localPort
        << ", Remote: " << remoteIp->toString() << ":" << remotePort << "\n";

    std::string line;

    // 메인 스레드는 사용자 입력을 계속 받습니다.
    while (running)
    {
        std::cout << "> " << std::flush;

        // 한 줄 입력을 받습니다.
        // 입력 스트림이 닫히면 루프를 종료합니다.
        if (!std::getline(std::cin, line))
        {
            break;
        }

        // /quit를 입력하면 프로그램을 종료합니다.
        if (line == "/quit")
        {
            running = false;
            break;
        }

        // UDP는 연결을 맺지 않고,
        // 보낼 때마다 목적지 IP와 포트를 직접 지정해서 데이터를 전송합니다.
        if (socket.send(line.c_str(), line.size(), *remoteIp, remotePort) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to send message.\n";
        }
    }

    // 종료 플래그를 내려서 수신 스레드도 빠져나오게 합니다.
    running = false;

    // 수신 스레드가 완전히 끝날 때까지 기다립니다.
    receiver.join();

    // 바인딩했던 로컬 포트를 해제합니다.
    socket.unbind();

    return 0;
}