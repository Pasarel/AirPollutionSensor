#include <iostream>
#include <sstream>
#include <map>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

string sendRequest(string method, string url, string body, map<string,string> headers) {
	try
	{
		URI uri(url);
		HTTPSClientSession session(uri.getHost(), uri.getPort());
		
		string path(uri.getPathAndQuery());
		if (path.empty()) path = "/";
		
		if (method == "POST") {
			HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);
			req.setContentType("application/x-www-form-urlencoded");
			
			for(map<string,string>::iterator it = headers.begin();
				it != headers.end(); it++) {
				req.set(it->first, it->second);
			}
		
			req.setContentLength( body.length() );
			std::ostream& os = session.sendRequest(req);
			os << body;  // sends the body
		}
		else if (method == "GET") {
			HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
			session.sendRequest(req);
		}
		
		HTTPResponse res;
		cout << res.getStatus() << " " << res.getReason() << endl;
		
		istream &is = session.receiveResponse(res);
		stringstream ss;
		StreamCopier::copyStream(is, ss);
		
		return ss.str();
	}
	catch (Exception &ex)
	{
		cerr << ex.displayText() << endl;
		return "";
	}
}

//--------------------------------------------------------------
int main(){
	string body = "key=d09668ea-def5-44ea-8c77-ae32e9fa5572";
	map<string,string> headers;
	headers["Content-Type"] = "application/x-www-form-urlencoded";
	//cout << sendRequest("POST", "https://apps.roiot.ro/aerlive/api/data.php", body, headers);
	cout << "test";
	cout << sendRequest("GET", "https://google.com", body, headers);
	return 0;
}