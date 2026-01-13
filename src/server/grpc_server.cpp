#include "grpc_server.h"

void GrpcServer::run(uint16_t port)
{
	std::string serverAddress = m_ip + ":" + std::to_string(port);

	grpc::ServerBuilder builder;
	builder.AddListeningPort(serverAddress, grpc::InsecureServerCredentials());
	builder.RegisterService(&m_grpcService);

	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	std::cout << "[gRPC Server] Listening on " << serverAddress << "\n";

	server->Wait();
}
