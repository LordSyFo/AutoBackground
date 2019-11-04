#include "Request.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <ctime>
#include <sstream>
#include <string>
#include <codecvt>
#include <locale>
#include <fstream>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

#define MAX_TIMEOUT_COUNT 100

using namespace AG;

Request::Request() : delayCounter_(0)
{
}

weatherInfo Request::getTodaysWeather() {

	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes("tmp.html")).then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		std::cout << "Requesting " << "https://www.skigastein.com/en/up-to-date/weather" << std::endl;
		http_client client(U("https://www.skigastein.com/en/up-to-date/weather"));

		// Build request URI and start the request.
		//uri_builder builder(U("cams/23/"));
		//builder.append_query(U("q"), U("cpprestsdk github"));

		return client.request(methods::GET);
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		response_ = response;

		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})

		// Close the file stream.
		.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	if (response_.status_code() != 200) {
		std::cout << "An error occured while trying to lookup weather.." << std::endl;
		return weatherInfo(0,0,0,0,0,0);
	}

	return parseSkiGastein("tmp.html");
}

weatherInfo Request::parseSkiGastein(std::string html_file)
{
	std::ifstream infile(html_file);
	std::string line;
	weatherInfo temp(0,0,0,0,0,0);

	if (infile.is_open()) {
		while (getline(infile, line))
		{
			if (line.find("<th>Summit</th>") != std::string::npos)
			{
				getline(infile, line);
				size_t start = line.find("<td>");
				if (start != std::string::npos)
					temp.temperatureSummit = atoi((line.substr(start+4, line.find("</td>")-start)).c_str());

				getline(infile, line);
				start = line.find("<td>");
				if (start != std::string::npos)
					temp.snowDepthSummit = atoi((line.substr(start+4, line.find("<sup>")-start)).c_str());

			}

			if (line.find("<th>Middle</th>") != std::string::npos)
			{
				getline(infile, line);
				size_t start = line.find("<td>");
				if (start != std::string::npos)
					temp.temperatureMiddle = atoi((line.substr(start + 4, line.find("</td>") - start)).c_str());

				getline(infile, line);
				start = line.find("<td>");
				if (start != std::string::npos)
					temp.snowDepthMiddle = atoi((line.substr(start + 4, line.find("<sup>") - start)).c_str());

			}

			if (line.find("<th>Valley</th>") != std::string::npos)
			{
				getline(infile, line);
				size_t start = line.find("<td>");
				if (start != std::string::npos)
					temp.temperatureValley = atoi((line.substr(start + 4, line.find("</td>") - start)).c_str());

				getline(infile, line);
				start = line.find("<td>");
				if (start != std::string::npos)
					temp.snowDepthValley = atoi((line.substr(start + 4, line.find("<sup>") - start)).c_str());

				return temp;
			}
		}
	}

	return temp;
}

std::string Request::downloadNewestImage(int cam, std::string outputname,bool first)
{
	auto fileStream = std::make_shared<ostream>();
	
	if(first)
		currentTime_.update();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(outputname)).then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		std::stringstream url;
		url << "https://panodata4.panomax.com/cams/" << cam << "/" << currentTime_.to_string() << endOfURL;

		std::cout << "Requesting " << url.str() << std::endl;
		http_client client(uri(std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(url.str())));

		// Build request URI and start the request.
		//uri_builder builder(U("cams/23/"));
		//builder.append_query(U("q"), U("cpprestsdk github"));
		
		return client.request(methods::GET);
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		response_ = response;

		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})

		// Close the file stream.
		.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	if (response_.status_code() != 200) {
		std::cout << "Retrying with different time..." << std::endl;

		currentTime_--;
		if (timeoutCount_++ > MAX_TIMEOUT_COUNT)
			return "\0";
		return downloadNewestImage(cam, outputname, false);
	}

	return currentTime_.to_string();
}

Request::~Request()
{
}
