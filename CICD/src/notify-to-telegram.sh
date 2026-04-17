#!/bin/bash

UPTIME=10
TG_BOT_TOKEN="8165967461:AAFGebTH5IFvrhpsSCtw3PaFdYXbhYbFeEU"
CHAT_ID="-1002390290564"
TG_API_URL="https://api.telegram.org/bot$TG_BOT_TOKEN/sendMessage"
MESSAGE_TEXT="Result: $1 $2"

echo "$MESSAGE_TEXT\n"
curl -s --max-time $UPTIME -d "chat_id=$CHAT_ID&disable_web_page_preview=1&text=$MESSAGE_TEXT" $TG_API_URL
