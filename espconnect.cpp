/* ESPConnect functions
 * December 2016 @ opensprinkler.com
 *
 * This file is part of the OpenSprinkler library
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#if defined(ESP8266) || defined(ESP32)

#include "espconnect.h"

#if  defined(ESP32)
#include "esp32.h"
#endif

String scan_network() {
	WiFi.mode(WIFI_STA);
	WiFi.disconnect();
	unsigned char n = WiFi.scanNetworks();
	String json;
	if (n>40) n = 40; // limit to 40 ssids max
	// maintain old format of wireless network JSON for mobile app compat
	json = "{\"ssids\":[";
	for(int i=0;i<n;i++) {
		json += "\"";
		json += WiFi.SSID(i);
		json += "\"";
		if(i<n-1) json += ",";
	}
	json += "],";
	// scanned contains complete wireless info including bssid and channel
	json += "\"scanned\":[";
	for(int i=0;i<n;i++) {
		json += "[\"" + WiFi.SSID(i) + "\",";
		json += "\"" + WiFi.BSSIDstr(i) + "\",";
		json += String(WiFi.RSSI(i))+",",
		json += String(WiFi.channel(i))+"]";
		if(i<n-1) json += ",";
	}
	json += "]}";
	return json;
}

void start_network_ap(const char *ssid, const char *pass) {
	if(!ssid || ssid[0] == '\0') return;   // FIX: was ssid == "\0" (pointer compare, never true)

	DEBUG_PRINT("SSID: '");
	DEBUG_PRINT(ssid);
	DEBUG_PRINTLN("'");

	// Set AP_STA mode BEFORE softAP: calling WiFi.mode() AFTER WiFi.softAP() on the
	// newer espressif32 core re-configures the interface and additionally spawns the
	// hardware-default "ESP_<mac>" AP alongside the firmware's "OS_<mac>" one (the
	// duplicate-SSID bug). Setting mode first yields exactly one softAP, while still
	// allowing a later STA connection for onboarding.
	WiFi.mode(WIFI_AP_STA);
	if(pass) WiFi.softAP(ssid, pass);
	else WiFi.softAP(ssid);
	DEBUG_PRINT(F("Starting AP with SSID "));
	DEBUG_PRINTLN(ssid);
}

void start_network_sta_with_ap(const char *ssid, const char *pass, int32_t channel, const unsigned char *bssid) {
	if(!ssid || !pass) return;
	if(WiFi.getMode()!=WIFI_AP_STA) WiFi.mode(WIFI_AP_STA);
	DEBUG_PRINT(F("Connecting in AP_STA to WiFi network "));
	DEBUG_PRINTLN(ssid);
	WiFi.begin(ssid, pass, channel, bssid);
}

void start_network_sta(const char *ssid, const char *pass, int32_t channel, const unsigned char *bssid) {
	if(!ssid || !pass) return;
	if(WiFi.getMode()!=WIFI_STA) WiFi.mode(WIFI_STA);
#if defined(ESP32)
	WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
	#if defined(MDNS_NAME)
	WiFi.setHostname(MDNS_NAME);
	#endif
	WiFi.setSleep(WIFI_PS_NONE);
#else
	WiFi.setSleep(false); // work-around for ARP issue: disable sleep mode
	WiFi.setOutputPower(20.5);
#endif
	DEBUG_PRINT(F("Connecting in STA to WiFi network "));
	DEBUG_PRINTLN(ssid);
	#if defined(ENABLE_WIFI_ROAMING)
	WiFi.begin(ssid, pass); // don't bind to channel/bssid, allow roaming!
	#else
	WiFi.begin(ssid, pass, channel, bssid);
	WiFi.setAutoReconnect(true); // enable auto reconnect
	#endif
}
#endif
