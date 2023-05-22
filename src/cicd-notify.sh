#!/bin/bash

declare URL="https://api.telegram.org/bot6189934117:AAH1HMmSt6Oj9ZLTzqUcbS1gPAyEWqXzitk/sendMessage"
declare STATUS=$(if [ $CI_JOB_STATUS = "success" ]; then echo -e "\xE2\x9C\x85"; else echo -e $"\xE2\x9D\x8C"; fi)
declare TEXT="$1%0A%0AJob:+$CI_JOB_NAME%0AStatus:+$STATUS%0AProject:+$CI_PROJECT_NAME%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s --max-time 10 -d "chat_id=1869162919&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null