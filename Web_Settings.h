#ifndef Web_Settings_h
#define Web_Settings_h

const char WebSettings[] PROGMEM = {
"<html>"
"<head>"
"<title>Settings</title>"
"</head>"
"<body>"
"<center>"
"<table border='0'>"
"<tr><td align='right' valign='top'>Current version:</td><td align='left' valign='top'>%_CURRENT_VERSION_%</td></tr>"
"<tr><td align='right' valign='top'>Board name:</td><td align='left' valign='top'>%_DEVICE_NAME_%</td></tr>"
"<tr><td align='right' valign='top'>Build options:</td><td align='left' valign='top'>%_CURRENT_OPTIONS_%</td></tr>"
"<tr><td align='right' valign='top'>Button settings:</td><td align='left' valign='top'>%_CURRENT_BUTTONS_%</td></tr>"
"<tr><td align='right' valign='top'>Various settings:</td><td align='left' valign='top'>%_CURRENT_VARIOUS_%</td></tr>"
"</table>"
"</center>"
"</body>"
"</html>"};

#endif
