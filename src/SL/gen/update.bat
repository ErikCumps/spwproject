@echo off

perl buildlist.pl init_ids.blst init_ids -C ../src -I ../include/SL -H SL
perl buildlist.pl message_ids.blst message_ids -C ../src -I ../include/SL -H SL
