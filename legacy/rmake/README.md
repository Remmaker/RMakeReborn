\# legacy/


Here lies the original RMake, written in raw C between around 2019-2021, with some maintain around 2023-2024.


\- No <string.h> used (except snprintf)

\- Hand-rolled strlen, strcmp, trim (with goto), split, lower…

\- Fixed buffers everywhere, manual linked lists, global state

\- Yet it already did builds, separate .o, hooks, caching, globbing


I wrote this alone, at night, crying blood.

This folder is a museum of pain.  

Do not touch. Do not compile. Do not judge.

RMake Reborn is my therapy.
