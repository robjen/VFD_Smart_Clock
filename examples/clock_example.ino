/*********************************************************************

  Ticking clock example for Wemos D1 Mini

  by Rob Jennings  07/01/2019
  
  also includes very simple web page to show that wifi stack is running
  the web page is really rough cut streaming direct out the HTTP

  the AP usually defaults to 192.168.4.1
  so join network and browse to that and tada ... 
  
*********************************************************************/


#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
/* you need to install a library ... TimeAlarms */
/* look at library   Time   by the same author for NTP and other utilities */
#include <Time.h>
#include <TimeAlarms.h>


#define HOST_NAME  "simpleClock"
#define MY_AP_NAME "simpleClock"
#define MY_AP_PASS "needspassword"  /* 8 characters minimum on this string */

#define LONGEST_LOOP_TIME  100  /* 100ms longest wait in main ... */


ESP8266WebServer server ( 80 );
ESP8266HTTPUpdateServer httpUpdater;


bool showClock = true;  /* you might not want to sometimes */


/************************************************************* 
 *  
 * here is your regular tick task called every second
 * once all the setup in complete
 * 
*************************************************************/
void ticktask(void)
{
  static time_t time_local;  // read once or it might change while you are here
  static char time_string[20];   // static so you are not hammering the stack in and out of this routine

  time_local = now(); // sample the time


  /* do you want to show time or you might be doing something else eg RSS feed display */
  if(showClock)
  {
    
    /* notice I set up your serial port to 9600 ... same as smart sockets accept */
    /* so you could fiddle that string below to tell the time on your socket string !!! */
    /* BUT set them up first and turn off effects perhaps when scrolling */
    /* sprintf("$B7M%02d:%02d:%02d",hour(time_local),minute(time_local),second(time_local))    that would do it ! */

    sprintf(time_string,"Time %02d:%02d:%02d",hour(time_local),minute(time_local),second(time_local));
    Serial.println(time_string);

  }
  
  /* even if we are not showing the clock poll the http interface */
  /* once a second is sluggish you might poll this from your main loop instead? */
  /* but leaving it here is nice as gets done in 'background' to your other stuff */
  server.handleClient();
}




/************************************************************* 
 *  
 * all set up stuff
 * 
*************************************************************/
void setup()   
{
  
  Serial.begin(9600); /* same speed as smart sockets ... */
  
  /* let things settle down after boot */
  delay(100);
  /* introducing your very important yield command .... keep feeding the stack */
  yield;


  /* set up your sockets ? few of these with the right strings to set up your sockets and your away */
  /* Serial.println(" stuff "); */


  WiFi.hostname(HOST_NAME);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(MY_AP_NAME, MY_AP_PASS);

  /* install 1 second tick tick timer */
  Alarm.timerRepeat(1, ticktask);

#ifdef DEBUG
  IPAddress myIP = WiFi.softAPIP();
  Serial.print(F("AP IP address: "));  // if I’m connected to something
  Serial.println(myIP);                // my IP as an access point otherwise 
#endif

  /* setup web page services */
  server.on ( "/", pageRoot );
  server.onNotFound ( handleNotFound );
  httpUpdater.setup(&server);
  server.begin();

#ifdef DEBUG
  Serial.println ( "HTTP server started" );
#endif


}





/************************************************************* 
 *  
 * main loop
 * 
 * could do all sort of stuff here ?
 * 
 * you MUST keep calling yield() to keep wifi alive and avoid watchdog
 *
 * you MUST use Alarm.delay(time) or the background tick process can’t run
 *
**************************************************************/

void loop()
{

  Serial.println("main loop starts ...");
  
  /* maybe you want to do NTP and RSS read or something */
  /* your clock is not set so will run from 00:00:00 */

  while(1)
  {

    yield(); /* each loop yield so that ESP does not crash */
    
    /*
      do your exciting stuff here !!!
      BUT don’t take too long doing it
      always yield() frequently if you must take a long time to do things
      and ALWAYS use Alarm.delay() instead of delay() so that tick can run
    */
    
    
    Alarm.delay(LONGEST_LOOP_TIME);  /* give other tasks time */
    
    /* busy doing nothing ... working the whole day through */
    /* trying to find lots of things not to do */
  }

}

/* web server root page */
void pageRoot() 
{
  static time_t time_local;  // might change while we spit out page
  static char time_string[20];   // static so you are not hammering the stack in and out of this routine

  time_local = now();

  sprintf(time_string,"Time %02d:%02d:%02d",hour(time_local),minute(time_local),second(time_local));
  
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), F("")); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(F("<html><head><meta http-equiv=\"refresh\" content=\"10\"><title>Simple Clock by Rob</title></head><body><h1>simple clock by Rob running</h1>"));
  server.sendContent( F("<p><h2>") );
  server.sendContent( time_string );
  server.sendContent( "</h2>" );
  
  server.sendContent( F("</body></html>") );
  server.client().stop(); // Stop is needed because we sent no content length

}


/* web server bad page */
void handleNotFound() 
{
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, F("text/html"), F("")); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  server.sendContent(F("<html><head></head><body><h1>404 Page Not Found</h1>"));
  server.sendContent( F("<p>URI: ") );
  server.sendContent( server.uri() );
  server.sendContent( F("</p><p>Method: ") );
  server.sendContent( ( server.method() == HTTP_GET ) ? "GET" : "POST" );
  server.sendContent( F("</p><p>Arguments: </p>") );

  for ( uint8_t i = 0; i < server.args(); i++ ) 
  {
    server.sendContent( F("<p>"));
    server.sendContent( server.argName(i) );
    server.sendContent( F(" : "));
    server.sendContent( server.arg(i) );
    server.sendContent( F("</p>"));
  }

  
  server.sendContent( F("</body></html>") );
  server.client().stop(); // Stop is needed because we sent no content length

}




