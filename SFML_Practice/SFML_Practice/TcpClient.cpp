#include <SFML/Network.hpp>

#include <cstdint>
#include <iostream>
#include <optional>
#include <string>

int main(int argc, char* argv[])
{
    // 실행 인자는 총 5개여야 합니다.
    // argv[0] : 실행 파일 이름
    // argv[1] : 서버 IP 주소
    // argv[2] : 서버 포트 번호
    // argv[3] : 플레이어 이름
    // argv[4] : 플레이어 점수
    if (argc != 5)
    {
        std::cout << "Usage: tcp_highscore_client <serverIp> <port> <name> <score>\n";
        return 1;
    }

    // 문자열로 들어온 서버 포트를 숫자로 변환합니다.
    const unsigned short serverPort = static_cast<unsigned short>(std::stoi(argv[2]));

    // 서버에 보낼 플레이어 이름입니다.
    const std::string playerName = argv[3];

    // 서버에 보낼 플레이어 점수입니다.
    const int playerScore = std::stoi(argv[4]);

    // 문자열 형태의 IP 주소를 sf::IpAddress로 변환합니다.
    // 변환이 실패할 수도 있으므로 optional로 받습니다.
    const std::optional<sf::IpAddress> serverIp = sf::IpAddress::fromString(argv[1]);

    // IP 주소가 잘못되었으면 더 진행하지 않고 종료합니다.
    if (!serverIp)
    {
        std::cerr << "Invalid server IP address: " << argv[1] << "\n";
        return 1;
    }

    // TCP 통신에 사용할 소켓 객체를 생성합니다.
    sf::TcpSocket socket;

    // 지정한 서버 IP와 포트로 연결을 시도합니다.
    // TCP는 UDP와 달리 먼저 연결을 맺은 뒤 데이터를 주고받습니다.
    if (socket.connect(*serverIp, serverPort) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to connect to server.\n";
        return 1;
    }

    // 서버에 보낼 데이터를 담을 패킷 객체입니다.
    // 패킷은 여러 값을 한 덩어리로 직렬화해서 전송할 때 편리합니다.
    sf::Packet request;

    // 플레이어 이름과 점수를 패킷에 순서대로 넣습니다.
    // 서버는 이 순서 그대로 꺼내야 정상적으로 읽을 수 있습니다.
    // TCP는 데이터 순서 보장과 신뢰성 있는 전송이 필요할 때 적합합니다.
    request << playerName << playerScore;

    // 준비한 패킷을 서버로 전송합니다.
    if (socket.send(request) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to send score.\n";
        return 1;
    }

    // 서버가 보내주는 응답 패킷을 받을 객체입니다.
    // 여기에는 현재 상위 점수표가 들어올 예정입니다.
    sf::Packet response;

    // 서버로부터 랭킹 데이터를 수신합니다.
    if (socket.receive(response) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to receive leaderboard.\n";
        return 1;
    }

    // 서버가 먼저 보내준 "랭킹 개수"를 저장할 변수입니다.
    // 예: 상위 5명까지면 count에 5가 들어옵니다.
    std::uint32_t count = 0;

    // 응답 패킷의 첫 번째 값으로 개수를 꺼냅니다.
    response >> count;

    std::cout << "Top Scores\n";

    // count만큼 반복하면서 이름과 점수를 차례대로 읽어옵니다.
    for (std::uint32_t i = 0; i < count; ++i)
    {
        // 한 명의 이름과 점수를 받을 임시 변수입니다.
        std::string name;
        int score = 0;

        // 서버가 넣은 순서대로 이름, 점수를 꺼냅니다.
        response >> name >> score;

        // 순위 번호는 0부터 시작하는 i에 1을 더해서 출력합니다.
        std::cout << (i + 1) << ". " << name << " - " << score << "\n";
    }

    // main 함수가 끝나면 socket 객체도 함께 정리됩니다.
    return 0;
}