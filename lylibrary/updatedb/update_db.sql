ALTER TABLE articoli MODIFY COLUMN cod_articolo INT(11) DEFAULT NULL auto_increment;
ALTER TABLE azienda MODIFY COLUMN id INT(20) DEFAULT NULL auto_increment;
ALTER TABLE biblioteca MODIFY COLUMN id INT(11) DEFAULT NULL auto_increment;
ALTER TABLE clienti MODIFY COLUMN id INT(11) DEFAULT NULL auto_increment;
ALTER TABLE prestiti MODIFY COLUMN id INT(11) DEFAULT NULL auto_increment;

DROP TABLE IF EXISTS `impostazioni`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `impostazioni` (
  `id` INTEGER(20) DEFAULT NULL,
  `impofontnameeditor` char(200) DEFAULT NULL,
  `fontsizeeditor` char(200) DEFAULT NULL,
  `impofontnameapplication` char(200) DEFAULT NULL,
  `fontsizeapplication` char(200) DEFAULT NULL,
  `language` char(100) DEFAULT NULL,
  `ipdatabase` char(200) DEFAULT NULL,
  `portdatabase` char(200) DEFAULT NULL,
  `namedatabase` char(200) DEFAULT NULL,
  `userdatabase` char(200) DEFAULT NULL,
  `passworddatabase` char(200) DEFAULT NULL,
  `enableupdate` char(35) DEFAULT NULL,
  `disableupdate` char(35) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
