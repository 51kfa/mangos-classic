#!/bin/bash

/usr/local/mysql/bin/mysql -uroot -p123456 < /data/wow/sql/create/db_create_mysql.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 < /data/wow/src/bindings/ScriptDev2/sql/scriptdev2_create_database.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 scriptdev2 < /data/wow/src/bindings/ScriptDev2/sql/scriptdev2_create_structure_mysql.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 characters < /data/wow/sql/base/characters.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 mangos < /data/wow/sql/base/mangos.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 realmd < /data/wow/sql/base/realmd.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 mangos < /data/wow/classicdb/Full_DB/ClassicDB_1_5_z2656.sql;
#/usr/local/mysql/bin/mysql -uroot -p123456 mangos < /data/wow/src/bindings/ScriptDev2/sql/mangos_scriptname_full.sql;
#/usr/local/mysql/bin/mysql -uroot -p123456 mangos < /data/wow/acid/acid_classic.sql;
#/usr/local/mysql/bin/mysql -uroot -p123456 mangos < /data/wow/classicdb/updates/update.sql;
/usr/local/mysql/bin/mysql -uroot -p123456 scriptdev2 < /data/wow/src/bindings/ScriptDev2/sql/scriptdev2_script_full.sql
