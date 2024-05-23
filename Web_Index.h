#ifndef Web_Index_h
#define Web_Index_h

#include "BMW_svg_gz.h"

const char WebIndex[] PROGMEM = {
"<html>"
"<head>"
"<title>BMW RT Remote</title>"
"</head>"
"<body>"
"<center>"
"<h1>BMW RT Remote</h1>"
"<img src='./BMW' alt='BMW logo'>"
"<h3>Update firmware:</h3>"
"<iframe src='./fwupdate' width='100%' height='175' style='border:none;'></iframe>"
"<hr width='50%'>"
"<h3>Current settings:</h3>"
"<iframe src='./settings' width='100%' height='350' style='border:none;'></iframe>"
"<form action='./restart'>"
  "<input type='submit' value='Restart'>"
"</form>"
"&copy;2022-2023 <a href='mailto:peter@luijer.org?subject=About BMW Remote'>Peter Luijer</a>"
"</center>"
"</body>"
"</html>"};

const char WebRestart[] PROGMEM = {
"<html>"
"<head>"
"<title>BMW RT Remote</title>"
"</head>"
"<body>"
"<center>"
"<h2>Restarting device...</h2>"
"You can close this window now"
"</center>"
"</body>"
"</html>"};

#endif
