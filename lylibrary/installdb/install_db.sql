-- MySQL dump 10.15  Distrib 10.0.21-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: lylibrary
-- ------------------------------------------------------
-- Server version	10.0.21-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `lylibrary`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `lylibrary` /*!40100 DEFAULT CHARACTER SET utf8 */;

USE `lylibrary`;

--
-- Table structure for table `aliquota`
--

DROP TABLE IF EXISTS `aliquota`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `aliquota` (
  `id` int(11) NOT NULL,
  `iva` char(40) NOT NULL,
  `descrizione` char(40) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `aliquota`
--

LOCK TABLES `aliquota` WRITE;
/*!40000 ALTER TABLE `aliquota` DISABLE KEYS */;
INSERT INTO `aliquota` VALUES (1,'21','Iva del 21%'),(2,'10','Iva del 10%'),(3,'4','Iva del 4%'),(4,'Esente IVA art.15','Esente IVA art. 15'),(5,'Non imponibile art. 2','Non imponibile art. 2'),(6,'Non imponibile art.41 D.L 513','Non imponibile art.41 D.L 513'),(7,'Esente IVA Art. 1 Fin.2008','Esente IVA Art. 1 Fin.2008'),(8,'Non imponibile art.8','Non imponibile art.8');
/*!40000 ALTER TABLE `aliquota` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `articoli`
--

DROP TABLE IF EXISTS `articoli`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `articoli` (
  `cod_articolo` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `articoli`
--

LOCK TABLES `articoli` WRITE;
/*!40000 ALTER TABLE `articoli` DISABLE KEYS */;
/*!40000 ALTER TABLE `articoli` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `azienda`
--

DROP TABLE IF EXISTS `azienda`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `azienda` (
  `id` INTEGER(20) NOT NULL AUTO_INCREMENT,
  `nome_azienda` char(20) NOT NULL,
  `partita_iva` char(30) NOT NULL,
  `cod_fisc` char(40) NOT NULL,
  `indirizzo` char(20) NOT NULL,
  `cap` char(5) NOT NULL,
  `localita` char(20) NOT NULL,
  `prov` char(2) NOT NULL,
  `telefono` char(20) NOT NULL,
  `fax` char(20) NOT NULL,
  `sito` char(40) NOT NULL,
  `email` char(35) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `azienda`
--

LOCK TABLES `azienda` WRITE;
/*!40000 ALTER TABLE `azienda` DISABLE KEYS */;
/*!40000 ALTER TABLE `azienda` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `banca`
--

DROP TABLE IF EXISTS `banca`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `banca` (
  `id` int(11) NOT NULL,
  `iban` char(40) NOT NULL,
  `nome_banca` char(40) NOT NULL,
  `cap` char(5) NOT NULL,
  `indirizzo` char(30) NOT NULL,
  `comune` char(40) NOT NULL,
  PRIMARY KEY (`iban`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `banca`
--

LOCK TABLES `banca` WRITE;
/*!40000 ALTER TABLE `banca` DISABLE KEYS */;
/*!40000 ALTER TABLE `banca` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `biblioteca`
--

DROP TABLE IF EXISTS `biblioteca`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `biblioteca` (
  `id` int(11) NOT NULL,
  `cliente` varchar(300) NOT NULL,
  `libro` varchar(300) NOT NULL,
  `data_prestito` date NOT NULL,
  `ora_prestito` char(20) NOT NULL,
  `data_rientro` date NOT NULL,
  `ora_rientro` char(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `biblioteca`
--

LOCK TABLES `biblioteca` WRITE;
/*!40000 ALTER TABLE `biblioteca` DISABLE KEYS */;
/*!40000 ALTER TABLE `biblioteca` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `biblioteca_dvd`
--

DROP TABLE IF EXISTS `biblioteca_dvd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `biblioteca_dvd` (
  `id` int(11) NOT NULL,
  `cliente` varchar(300) NOT NULL,
  `dvd` varchar(300) NOT NULL,
  `data_prestito` date NOT NULL,
  `ora_prestito` char(20) NOT NULL,
  `data_rientro` date NOT NULL,
  `ora_rientro` char(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `biblioteca_dvd`
--

LOCK TABLES `biblioteca_dvd` WRITE;
/*!40000 ALTER TABLE `biblioteca_dvd` DISABLE KEYS */;
/*!40000 ALTER TABLE `biblioteca_dvd` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `carico_libri`
--

DROP TABLE IF EXISTS `carico_libri`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `carico_libri` (
  `cod_articolo` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `pr_unit` decimal(10,2) NOT NULL,
  `pr_s_iva` decimal(10,2) NOT NULL,
  `pr_c_iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `carico_libri`
--

LOCK TABLES `carico_libri` WRITE;
/*!40000 ALTER TABLE `carico_libri` DISABLE KEYS */;
/*!40000 ALTER TABLE `carico_libri` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `carico_prod_dig`
--

DROP TABLE IF EXISTS `carico_prod_dig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `carico_prod_dig` (
  `cod_articolo` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `pr_unit` decimal(10,2) NOT NULL,
  `pr_s_iva` decimal(10,2) NOT NULL,
  `pr_c_iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `carico_prod_dig`
--

LOCK TABLES `carico_prod_dig` WRITE;
/*!40000 ALTER TABLE `carico_prod_dig` DISABLE KEYS */;
/*!40000 ALTER TABLE `carico_prod_dig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `categoria`
--

DROP TABLE IF EXISTS `categoria`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categoria` (
  `categ` int(11) NOT NULL,
  `tipo_categ` varchar(40) NOT NULL,
  PRIMARY KEY (`categ`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categoria`
--

LOCK TABLES `categoria` WRITE;
/*!40000 ALTER TABLE `categoria` DISABLE KEYS */;
INSERT INTO `categoria` VALUES (1,'Architettura'),(2,'Design'),(3,'Musica'),(4,'Storia dell\' arte'),(5,'Cinema e televisione'),(6,'Fotografia'),(7,'Pittura'),(8,'Teatro'),(9,'Danza'),(10,'Musei'),(11,'Scultura'),(12,'Economia'),(13,'Industria e studi di settore'),(14,'Diritto'),(15,'Management'),(16,'Finanza e contabilit� '),(17,'Certificazione informartica'),(18,'Hardware'),(19,'Reti e network'),(20,'Sistemi operativi'),(21,'Database'),(22,'Internet & web'),(23,'Scienza dei calcolatori'),(24,'Software per ufficio'),(25,'Grafica e multimedia'),(26,'Programmazione'),(27,'Sicurezza informatica'),(28,'Animali'),(29,'Giochi e quiz'),(30,'Modellismo'),(31,'Videogiochi'),(32,'Collezionismo'),(33,'Cucina'),(34,'Trasporti'),(35,'Giardinaggio'),(36,'Video'),(37,'Antologie'),(38,'Saggistica'),(39,'Letteratura teatrale'),(40,'Poesia'),(41,'Libri illustrati'),(42,'Narrativa'),(43,'Classici'),(44,'Fiabe'),(45,'Narrativa storica'),(46,'Poesia'),(47,'Antologia'),(48,'Fantascienza'),(49,'Giallo & thriller'),(50,'Racconti'),(51,'Primo apprendimento'),(52,'Avventura'),(53,'Fantasy'),(54,'Horror'),(55,'Grammatica'),(56,'Linguistica'),(57,'Dizionari'),(58,'Romanzi rosa'),(59,'Miti, saghe e leggende'),(60,'Storia'),(61,'Italiano'),(62,'Matematica');
/*!40000 ALTER TABLE `categoria` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `causali`
--

DROP TABLE IF EXISTS `causali`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `causali` (
  `id` int(11) NOT NULL,
  `nome_causale` char(40) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `causali`
--

LOCK TABLES `causali` WRITE;
/*!40000 ALTER TABLE `causali` DISABLE KEYS */;
INSERT INTO `causali` VALUES (1,'C/LAVORAZIONE'),(2,'C/RIPARAZIONE'),(3,'C/VISIONE'),(4,'CESSIONE'),(5,'CONSEGNA C/TERZI'),(6,'LAVORAZIONE'),(7,'OMAGGIO'),(8,'RESO'),(9,'RESO C/LAVORAZIONE'),(10,'RESO C/RIPARAZIONE'),(11,'RESO C/VISIONE'),(12,'RESO SCARTO INUTILIZZABILE'),(13,'RIPARAZIONE'),(14,'TENTATA VENDITA'),(15,'VENDITA');
/*!40000 ALTER TABLE `causali` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `clienti`
--

DROP TABLE IF EXISTS `clienti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clienti` (
  `id` int(11) NOT NULL,
  `nome` char(30) NOT NULL,
  `cognome` char(30) NOT NULL,
  `indirizzo` char(30) NOT NULL,
  `telefono` char(30) NOT NULL,
  `email` char(30) NOT NULL,
  `cod_fisc` char(18) DEFAULT NULL,
  `part_iva` char(11) DEFAULT NULL,
  `fax` char(15) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clienti`
--

LOCK TABLES `clienti` WRITE;
/*!40000 ALTER TABLE `clienti` DISABLE KEYS */;
/*!40000 ALTER TABLE `clienti` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fattura_vendita`
--

DROP TABLE IF EXISTS `fattura_vendita`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fattura_vendita` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(50) NOT NULL,
  `tipo_fattura` varchar(100) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fattura_vendita`
--

LOCK TABLES `fattura_vendita` WRITE;
/*!40000 ALTER TABLE `fattura_vendita` DISABLE KEYS */;
/*!40000 ALTER TABLE `fattura_vendita` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fattura_vendita_pr_dig`
--

DROP TABLE IF EXISTS `fattura_vendita_pr_dig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fattura_vendita_pr_dig` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(50) NOT NULL,
  `tipo_fattura` varchar(100) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fattura_vendita_pr_dig`
--

LOCK TABLES `fattura_vendita_pr_dig` WRITE;
/*!40000 ALTER TABLE `fattura_vendita_pr_dig` DISABLE KEYS */;
/*!40000 ALTER TABLE `fattura_vendita_pr_dig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_acq`
--

DROP TABLE IF EXISTS `fatture_acq`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_acq` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(50) NOT NULL,
  `tipo_fattura` varchar(100) NOT NULL,
  `totale` decimal(10,2) NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_acq`
--

LOCK TABLES `fatture_acq` WRITE;
/*!40000 ALTER TABLE `fatture_acq` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_acq` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_acq_righe`
--

DROP TABLE IF EXISTS `fatture_acq_righe`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_acq_righe` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(200) NOT NULL,
  `sconto_1` decimal(10,2) NOT NULL,
  `sconto_2` decimal(10,2) NOT NULL,
  `sconto_3` decimal(10,2) NOT NULL,
  `spese_trasporto` decimal(10,2) NOT NULL,
  `spese_incasso` decimal(10,2) NOT NULL,
  `tipo_pagamento` char(200) NOT NULL,
  `numero_fattura` char(200) NOT NULL,
  `data_fattura` date NOT NULL,
  `note_fattura` char(40) NOT NULL,
  `prezzo_unit` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_acq_righe`
--

LOCK TABLES `fatture_acq_righe` WRITE;
/*!40000 ALTER TABLE `fatture_acq_righe` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_acq_righe` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_righe_acq_art`
--

DROP TABLE IF EXISTS `fatture_righe_acq_art`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_righe_acq_art` (
  `id` int(10) unsigned NOT NULL,
  `cod_art` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `unita` char(5) NOT NULL,
  `prezzo_unit` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `prezzo_c_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `sconto` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_righe_acq_art`
--

LOCK TABLES `fatture_righe_acq_art` WRITE;
/*!40000 ALTER TABLE `fatture_righe_acq_art` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_righe_acq_art` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_righe_vendita_art`
--

DROP TABLE IF EXISTS `fatture_righe_vendita_art`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_righe_vendita_art` (
  `id` int(10) unsigned NOT NULL,
  `cod_art` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `unita` char(5) NOT NULL,
  `prezzo_unit` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `prezzo_c_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `sconto` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_righe_vendita_art`
--

LOCK TABLES `fatture_righe_vendita_art` WRITE;
/*!40000 ALTER TABLE `fatture_righe_vendita_art` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_righe_vendita_art` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_righe_vendita_prod_dig`
--

DROP TABLE IF EXISTS `fatture_righe_vendita_prod_dig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_righe_vendita_prod_dig` (
  `id` int(10) unsigned NOT NULL,
  `cod_art` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `unita` char(5) NOT NULL,
  `prezzo_unit` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `prezzo_c_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `sconto` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_righe_vendita_prod_dig`
--

LOCK TABLES `fatture_righe_vendita_prod_dig` WRITE;
/*!40000 ALTER TABLE `fatture_righe_vendita_prod_dig` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_righe_vendita_prod_dig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_vendita_righe`
--

DROP TABLE IF EXISTS `fatture_vendita_righe`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_vendita_righe` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(200) NOT NULL,
  `tipo_cliente` char(20) NOT NULL,
  `sconto_1` decimal(10,2) NOT NULL,
  `sconto_2` decimal(10,2) NOT NULL,
  `sconto_3` decimal(10,2) NOT NULL,
  `spese_trasporto` decimal(10,2) NOT NULL,
  `spese_incasso` decimal(10,2) NOT NULL,
  `annotazioni` char(120) NOT NULL,
  `vs_ordine` char(30) NOT NULL,
  `tipo_pagamento` char(200) NOT NULL,
  `note_pagamento` char(200) NOT NULL,
  `rag_soc` char(100) NOT NULL,
  `indirizzo` char(100) DEFAULT NULL,
  `cap` char(100) DEFAULT NULL,
  `localita` char(100) DEFAULT NULL,
  `prov` char(100) DEFAULT NULL,
  `telefono` char(200) DEFAULT NULL,
  `fax` char(200) DEFAULT NULL,
  `piva_cfis` char(200) DEFAULT NULL,
  `banca` char(100) NOT NULL,
  `iban` char(100) DEFAULT NULL,
  `agente` char(50) NOT NULL,
  `provvigione` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `scadenza_1` date DEFAULT NULL,
  `scadenza_2` date DEFAULT NULL,
  `scadenza_3` date DEFAULT NULL,
  `importo_1` decimal(10,2) DEFAULT NULL,
  `importo_2` decimal(10,2) DEFAULT NULL,
  `importo_3` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_vendita_righe`
--

LOCK TABLES `fatture_vendita_righe` WRITE;
/*!40000 ALTER TABLE `fatture_vendita_righe` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_vendita_righe` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fatture_vendita_righe_dig`
--

DROP TABLE IF EXISTS `fatture_vendita_righe_dig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fatture_vendita_righe_dig` (
  `id` int(10) unsigned NOT NULL,
  `data` date NOT NULL,
  `fornitore` char(200) NOT NULL,
  `tipo_cliente` char(20) NOT NULL,
  `sconto_1` decimal(10,2) NOT NULL,
  `sconto_2` decimal(10,2) NOT NULL,
  `sconto_3` decimal(10,2) NOT NULL,
  `spese_trasporto` decimal(10,2) NOT NULL,
  `spese_incasso` decimal(10,2) NOT NULL,
  `annotazioni` char(120) NOT NULL,
  `vs_ordine` char(30) NOT NULL,
  `tipo_pagamento` char(200) NOT NULL,
  `note_pagamento` char(200) NOT NULL,
  `rag_soc` char(100) NOT NULL,
  `indirizzo` char(100) DEFAULT NULL,
  `cap` char(100) DEFAULT NULL,
  `localita` char(100) DEFAULT NULL,
  `prov` char(100) DEFAULT NULL,
  `telefono` char(200) DEFAULT NULL,
  `fax` char(200) DEFAULT NULL,
  `piva_cfis` char(200) DEFAULT NULL,
  `banca` char(100) NOT NULL,
  `iban` char(100) DEFAULT NULL,
  `agente` char(50) NOT NULL,
  `provvigione` decimal(10,2) NOT NULL,
  `prezzo_s_iva` decimal(10,2) NOT NULL,
  `iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `scadenza_1` date DEFAULT NULL,
  `scadenza_2` date DEFAULT NULL,
  `scadenza_3` date DEFAULT NULL,
  `importo_1` decimal(10,2) DEFAULT NULL,
  `importo_2` decimal(10,2) DEFAULT NULL,
  `importo_3` decimal(10,2) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fatture_vendita_righe_dig`
--

LOCK TABLES `fatture_vendita_righe_dig` WRITE;
/*!40000 ALTER TABLE `fatture_vendita_righe_dig` DISABLE KEYS */;
/*!40000 ALTER TABLE `fatture_vendita_righe_dig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fornitori`
--

DROP TABLE IF EXISTS `fornitori`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fornitori` (
  `id` int(11) NOT NULL,
  `ragione_sociale` char(100) NOT NULL,
  `part_iva` char(12) NOT NULL,
  `cod_fiscale` char(16) NOT NULL,
  `indirizzo` char(20) NOT NULL,
  `cap` char(20) NOT NULL,
  `localita` char(30) NOT NULL,
  `prov` char(30) NOT NULL,
  `telefono` char(20) NOT NULL,
  `fax` char(20) NOT NULL,
  `email` char(30) NOT NULL,
  `sito` char(30) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fornitori`
--

LOCK TABLES `fornitori` WRITE;
/*!40000 ALTER TABLE `fornitori` DISABLE KEYS */;
/*!40000 ALTER TABLE `fornitori` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `idzero`
--

DROP TABLE IF EXISTS `idzero`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idzero` (
  `id` int(11) NOT NULL,
  `text` char(40) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `idzero`
--

LOCK TABLES `idzero` WRITE;
/*!40000 ALTER TABLE `idzero` DISABLE KEYS */;
INSERT INTO `idzero` VALUES (0,'');
/*!40000 ALTER TABLE `idzero` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `misura`
--

DROP TABLE IF EXISTS `misura`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `misura` (
  `id` int(11) NOT NULL,
  `unita` char(3) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `misura`
--

LOCK TABLES `misura` WRITE;
/*!40000 ALTER TABLE `misura` DISABLE KEYS */;
INSERT INTO `misura` VALUES (1,'qt'),(2,'mt'),(3,'kg'),(4,'Lt');
/*!40000 ALTER TABLE `misura` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `pagam`
--

DROP TABLE IF EXISTS `pagam`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `pagam` (
  `codice` varchar(50) NOT NULL,
  `descrizione` varchar(250) NOT NULL,
  `note_fattura` char(40) NOT NULL,
  PRIMARY KEY (`codice`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `pagam`
--

LOCK TABLES `pagam` WRITE;
/*!40000 ALTER TABLE `pagam` DISABLE KEYS */;
INSERT INTO `pagam` VALUES ('R.D.','Rimessa diretta',''),('BONIFICO BANCARIO','BONIFICO BANCARIO GENERICO',''),('BONIFICO BANCARIO (A)','BONIFICO BANCARIO (A)','Coordinate Bancarie per effettuare bonif'),('BONIFICO BANCARIO (B)','BONIFICO BANCARIO (B)','Coordinate Bancarie per effettuare bonif'),('RB 120','Ricevuta Bancaria 120 gg',''),('RB 120 FM','Ricevuta Bancaria 120 gg fine mese',''),('RB 150','Ricevuta Bancaria 150 gg',''),('RB 150 FM','Ricevuta Bancaria 150 gg fine mese',''),('RB 180','Ricevuta Bancaria 180 gg',''),('RB 180 FM','Ricevuta Bancaria 150 gg fine mese',''),('RB 30','Ricevuta Bancaria 30 gg',''),('RB 30 FM','Ricevuta Bancaria 30 gg fine mese',''),('RB 30/60','Ricevuta Bancaria 30/60 gg',''),('RB 30/60 FM','Ricevuta Bancaria 30/60 gg fine mese',''),('RB 30/60/90','Ricevuta Bancaria 30/60/90 gg',''),('RB 30/60/90 FM','Ricevuta Bancaria 30/60/90 gg fine mese',''),('RB 60','Ricevuta Bancaria 60 gg',''),('RB 60 FM','Ricevuta Bancaria 60 gg fine mese',''),('RB 60/90','Ricevuta Bancaria 60/90 gg',''),('RB 60/90 FM','Ricevuta Bancaria 60/90 gg fine mese',''),('RB 90','Ricevuta Bancaria 90 gg',''),('RB 90 FM','Ricevuta Bancaria 90 gg fine mese','');
/*!40000 ALTER TABLE `pagam` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `porto`
--

DROP TABLE IF EXISTS `porto`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `porto` (
  `id` int(11) NOT NULL,
  `tipo_di_porto` varchar(150) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `porto`
--

LOCK TABLES `porto` WRITE;
/*!40000 ALTER TABLE `porto` DISABLE KEYS */;
INSERT INTO `porto` VALUES (1,'Porto franco'),(2,'Porto assegnato');
/*!40000 ALTER TABLE `porto` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prestiti`
--

DROP TABLE IF EXISTS `prestiti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prestiti` (
  `id` int(11) NOT NULL,
  `cliente` varchar(300) NOT NULL,
  `libro` varchar(300) NOT NULL,
  `data_prestito` date NOT NULL,
  `data_rientro` date NOT NULL,
  `rientro` varchar(30) DEFAULT 'Non rientrato',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prestiti`
--

LOCK TABLES `prestiti` WRITE;
/*!40000 ALTER TABLE `prestiti` DISABLE KEYS */;
/*!40000 ALTER TABLE `prestiti` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prestiti_dvd`
--

DROP TABLE IF EXISTS `prestiti_dvd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prestiti_dvd` (
  `id` int(11) NOT NULL,
  `cliente` varchar(300) NOT NULL,
  `dvd` varchar(300) NOT NULL,
  `data_prestito` date NOT NULL,
  `data_rientro` date NOT NULL,
  `rientro` varchar(30) DEFAULT 'Non rientrato',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prestiti_dvd`
--

LOCK TABLES `prestiti_dvd` WRITE;
/*!40000 ALTER TABLE `prestiti_dvd` DISABLE KEYS */;
/*!40000 ALTER TABLE `prestiti_dvd` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `prodotti_dvd`
--

DROP TABLE IF EXISTS `prodotti_dvd`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prodotti_dvd` (
  `cod_prodotto` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_prodotto` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_prodotto`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prodotti_dvd`
--

LOCK TABLES `prodotti_dvd` WRITE;
/*!40000 ALTER TABLE `prodotti_dvd` DISABLE KEYS */;
/*!40000 ALTER TABLE `prodotti_dvd` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scarico_libri`
--

DROP TABLE IF EXISTS `scarico_libri`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scarico_libri` (
  `cod_articolo` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `pr_unit` decimal(10,2) NOT NULL,
  `pr_s_iva` decimal(10,2) NOT NULL,
  `pr_c_iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scarico_libri`
--

LOCK TABLES `scarico_libri` WRITE;
/*!40000 ALTER TABLE `scarico_libri` DISABLE KEYS */;
/*!40000 ALTER TABLE `scarico_libri` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scarico_prod_dig`
--

DROP TABLE IF EXISTS `scarico_prod_dig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `scarico_prod_dig` (
  `cod_articolo` int(11) NOT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) NOT NULL,
  `descrizione` char(150) NOT NULL,
  `autore` varchar(30) NOT NULL,
  `lingua` varchar(30) NOT NULL,
  `categ` varchar(200) NOT NULL,
  `scaffale` varchar(30) NOT NULL,
  `quantita` decimal(10,2) NOT NULL,
  `pr_unit` decimal(10,2) NOT NULL,
  `pr_s_iva` decimal(10,2) NOT NULL,
  `pr_c_iva` decimal(10,2) NOT NULL,
  `totale` decimal(10,2) NOT NULL,
  `infoeditore` varchar(200) NOT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scarico_prod_dig`
--

LOCK TABLES `scarico_prod_dig` WRITE;
/*!40000 ALTER TABLE `scarico_prod_dig` DISABLE KEYS */;
/*!40000 ALTER TABLE `scarico_prod_dig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `vettori`
--

DROP TABLE IF EXISTS `vettori`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `vettori` (
  `id` int(11) NOT NULL,
  `vett` varchar(200) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `vettori`
--

LOCK TABLES `vettori` WRITE;
/*!40000 ALTER TABLE `vettori` DISABLE KEYS */;
/*!40000 ALTER TABLE `vettori` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-30  8:24:37
