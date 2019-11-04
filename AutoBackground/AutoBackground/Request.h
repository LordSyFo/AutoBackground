#pragma once
#include <string>
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include "Time.h"

namespace AG {

	struct weatherInfo;

	class Request
	{
	public:
		Request();

		std::string downloadNewestImage(int cam, std::string outputname, bool first);

		weatherInfo getTodaysWeather();
		
		int round(int n);

		~Request();

	private:
		std::string timeStamp_;
		unsigned int delayCounter_;
		web::http::http_response response_;

		std::string getHourString(int hour);
		weatherInfo parseSkiGastein(std::string html_file);

		Time currentTime_;
		const std::string endOfURL = "_default.jpg";

		unsigned int timeoutCount_;
	};


	typedef struct weatherInfo {
		int snowDepthSummit
			, snowDepthMiddle
			, snowDepthValley
			, temperatureSummit
			, temperatureMiddle
			, temperatureValley;
		weatherInfo(int snowsum,
			int snowmid,
			int snowval,
			int tempsum,
			int tempmid,
			int tempval) :
			snowDepthSummit(snowsum)
			, snowDepthMiddle(snowmid)
			, snowDepthValley(snowval)
			, temperatureSummit(tempsum)
			, temperatureMiddle(tempmid)
			, temperatureValley(tempval) {};
		void print() {
			std::cout << "SUMMIT:\n"
				<< "Temperature: " << temperatureSummit << std::endl
				<< "Snow-depth: " << snowDepthSummit << std::endl;
			std::cout << "MIDDLE:\n"
				<< "Temperature: " << temperatureMiddle << std::endl
				<< "Snow-depth: " << snowDepthMiddle << std::endl;
			std::cout << "VALLEY:\n"
				<< "Temperature: " << temperatureValley << std::endl
				<< "Snow-depth: " << snowDepthValley << std::endl;
		}
	} weatherInfo;
}