-- message Adding relation table information to attribute table

ALTER TABLE attributes ADD COLUMN relationTable varchar(64) default NULL; -- AFTER valueIsList;
ALTER TABLE attributes ADD COLUMN relationIDColumn varchar(64) default NULL; --  AFTER relationTable;
ALTER TABLE attributes ADD COLUMN relationStringColumn varchar(64) default NULL; --  AFTER relationIDColumn;

