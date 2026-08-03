#include <SFML/Network.hpp>

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// 한 명의 점수 정보를 저장하기 위한 구조체입니다.
// 이름과 점수를 하나로 묶어서 관리하기 위해 사용합니다.
struct ScoreEntry
{
    std::string name; // 플레이어 이름
    int score = 0;    // 플레이어 점수
};

int main(int argc, char* argv[])
{
    // 서버가 수신 대기할 포트 번호입니다.
    // 실행 인자를 하나 이상 넣으면 argv[1]의 값을 포트로 사용하고,
    // 인자를 안 넣으면 기본값 55000 포트를 사용합니다.
    const unsigned short listenPort =
        (argc >= 2) ? static_cast<unsigned short>(std::stoi(argv[1])) : 55000;

    // TCP 서버 역할을 하는 리스너 객체입니다.
    // 클라이언트의 접속 요청을 받아들이는 역할을 합니다.
    sf::TcpListener listener;

    // 지정한 포트에서 접속 대기를 시작합니다.
    // 이 작업이 성공해야 클라이언트가 해당 포트로 접속할 수 있습니다.
    if (listener.listen(listenPort) != sf::Socket::Status::Done)
    {
        std::cerr << "Failed to listen on port " << listenPort << "\n";
        return 1;
    }

    // 서버가 관리할 점수표입니다.
    // 클라이언트가 점수를 보낼 때마다 이 벡터에 저장한 뒤 정렬합니다.
    std::vector<ScoreEntry> leaderboard;

    std::cout << "TCP high score server listening on port " << listenPort << "\n";

    // 서버는 계속 실행되면서 접속을 기다립니다.
    while (true)
    {
        // 클라이언트 한 명과 통신할 소켓입니다.
        // accept()가 성공하면 이 소켓이 실제 접속된 클라이언트를 나타냅니다.
        sf::TcpSocket client;

        // 클라이언트 접속 요청을 수락합니다.
        // 접속이 실패하면 에러를 출력하고 다음 루프로 넘어갑니다.
        if (listener.accept(client) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to accept client.\n";
            continue;
        }

        // 클라이언트가 보낸 데이터를 담을 패킷입니다.
        sf::Packet request;

        // 클라이언트로부터 이름과 점수가 담긴 패킷을 받습니다.
        if (client.receive(request) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to receive score packet.\n";
            continue;
        }

        // 클라이언트가 보낸 점수 정보를 저장할 임시 변수입니다.
        ScoreEntry incoming;

        // 패킷에서 이름과 점수를 꺼냅니다.
        // 클라이언트가 보낸 순서와 서버가 읽는 순서가 같아야 합니다.
        // 형식이 맞지 않으면 잘못된 패킷으로 판단합니다.
        if (!(request >> incoming.name >> incoming.score))
        {
            std::cerr << "Invalid packet format.\n";
            continue;
        }

        // 새로 들어온 점수를 점수표에 추가합니다.
        leaderboard.push_back(incoming);

        // 점수가 높은 사람부터 앞으로 오도록 내림차순 정렬합니다.
        // a.score > b.score 이므로 큰 점수가 먼저 배치됩니다.
        std::sort(leaderboard.begin(), leaderboard.end(),
            [](const ScoreEntry& a, const ScoreEntry& b)
            {
                return a.score > b.score;
            });

        // 점수표가 5명을 초과하면 뒤쪽(낮은 점수)을 잘라냅니다.
        // 즉 항상 상위 5개 점수만 유지합니다.
        if (leaderboard.size() > 5)
        {
            leaderboard.resize(5);
        }

        // 서버 콘솔에 방금 들어온 점수를 출력합니다.
        std::cout << incoming.name << " scored " << incoming.score << "\n";

        // 클라이언트에게 다시 보낼 응답 패킷입니다.
        // 여기에는 현재 상위 랭킹 정보가 담깁니다.
        sf::Packet response;

        // 먼저 점수표에 몇 명이 들어있는지 보냅니다.
        // 클라이언트는 이 개수만큼 반복해서 이름/점수를 읽게 됩니다.
        response << static_cast<std::uint32_t>(leaderboard.size());

        // 점수표의 각 항목을 순서대로 패킷에 넣습니다.
        // 정렬된 상태이므로 1등부터 차례대로 들어갑니다.
        for (const ScoreEntry& entry : leaderboard)
        {
            response << entry.name << entry.score;
        }

        // 준비한 랭킹 데이터를 클라이언트에게 전송합니다.
        if (client.send(response) != sf::Socket::Status::Done)
        {
            std::cerr << "Failed to send leaderboard.\n";
        }
    }
}