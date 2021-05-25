all: server crawler

server:
	g++ -std=c++17 src/sensorpollution.cpp src/Controller_AUTO_PROCESS.cpp src/Controller_MANUAL_PROCESS.cpp src/Viewer_MANUAL_PROCESS.cpp src/Viewer_AUTO_PROCESS.cpp src/Webhook.cpp src/Window_Model.cpp src/APIClient.cpp src/Image.cpp -lpistache -lcrypto -lssl -lpthread -lPocoNetSSL -lPocoNet -lPocoFoundation -o server.out

crawler:
	g++ -std=c++17 src/Crawler.cpp src/APIClient.cpp -lPocoNetSSL -lPocoNet -lPocoFoundation -o crawler.out