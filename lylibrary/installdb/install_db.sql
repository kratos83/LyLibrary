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
-- Table structure for table `biblioteca`
--

DROP TABLE IF EXISTS `biblioteca`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `biblioteca` (
  `id` int(11) DEFAULT NULL AUTO_INCREMENT,
  `cliente` varchar(300) DEFAULT NULL,
  `libro` varchar(300) DEFAULT NULL,
  `data_prestito` date DEFAULT NULL,
  `ora_prestito` char(20) DEFAULT NULL,
  `data_rientro` date DEFAULT NULL,
  `ora_rientro` char(20) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `biblioteca`
--

LOCK TABLES `biblioteca` WRITE;
/*!40000 ALTER TABLE `biblioteca` DISABLE KEYS */;
/*!40000 ALTER TABLE `biblioteca` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `articoli`
--

DROP TABLE IF EXISTS `articoli`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `articoli` (
  `cod_articolo` int(11) DEFAULT NULL  AUTO_INCREMENT,
  `codbarre` char(20) DEFAULT NULL,
  `nome_articolo` varchar(200) DEFAULT NULL,
  `descrizione` char(150) DEFAULT NULL,
  `autore` varchar(30) DEFAULT NULL,
  `lingua` varchar(30) DEFAULT NULL,
  `categ` varchar(200) DEFAULT NULL,
  `scaffale` varchar(30) DEFAULT NULL,
  `quantita` decimal(10,2) DEFAULT NULL,
  `infoeditore` varchar(200) DEFAULT NULL,
  `image` varchar(300) DEFAULT NULL,
  PRIMARY KEY (`cod_articolo`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
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
  `id` INTEGER(20) DEFAULT NULL AUTO_INCREMENT,
  `nome_azienda` char(20) DEFAULT NULL,
  `partita_iva` char(30) DEFAULT NULL,
  `cod_fisc` char(40) DEFAULT NULL,
  `indirizzo` char(20) DEFAULT NULL,
  `cap` char(5) DEFAULT NULL,
  `localita` char(20) DEFAULT NULL,
  `prov` char(2) DEFAULT NULL,
  `telefono` char(20) DEFAULT NULL,
  `fax` char(20) DEFAULT NULL,
  `sito` char(40) DEFAULT NULL,
  `email` char(35) DEFAULT NULL,
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

DROP TABLE IF EXISTS `categoria`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `categoria` (
  `categ` int(11) DEFAULT NULL,
  `tipo_categ` varchar(40) DEFAULT NULL,
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
-- Table structure for table `clienti`
--

DROP TABLE IF EXISTS `clienti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `clienti` (
  `id` int(11) DEFAULT NULL AUTO_INCREMENT,
  `nome` char(30) DEFAULT NULL,
  `cognome` char(30) DEFAULT NULL,
  `indirizzo` char(30) DEFAULT NULL,
  `telefono` char(30) DEFAULT NULL,
  `email` char(30) DEFAULT NULL,
  `cod_fisc` char(18) DEFAULT NULL,
  `part_iva` char(11) DEFAULT NULL,
  `fax` char(15) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `clienti`
--

LOCK TABLES `clienti` WRITE;
/*!40000 ALTER TABLE `clienti` DISABLE KEYS */;
/*!40000 ALTER TABLE `clienti` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `idzero`
--

DROP TABLE IF EXISTS `idzero`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `idzero` (
  `id` int(11) DEFAULT NULL,
  `text` char(40) DEFAULT NULL,
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
-- Table structure for table `prestiti`
--

DROP TABLE IF EXISTS `prestiti`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `prestiti` (
  `id` int(11) DEFAULT NULL AUTO_INCREMENT,
  `cliente` varchar(300) DEFAULT NULL,
  `libro` varchar(300) DEFAULT NULL,
  `data_prestito` date DEFAULT NULL,
  `data_rientro` date DEFAULT NULL,
  `rientro` varchar(30) DEFAULT 'Non rientrato',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
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
  `id` int(11) DEFAULT NULL,
  `cliente` varchar(300) DEFAULT NULL,
  `dvd` varchar(300) DEFAULT NULL,
  `data_prestito` date DEFAULT NULL,
  `data_rientro` date DEFAULT NULL,
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
  `cod_prodotto` int(11) DEFAULT NULL,
  `codbarre` char(20) DEFAULT NULL,
  `nome_prodotto` varchar(200) DEFAULT NULL,
  `descrizione` char(150) DEFAULT NULL,
  `autore` varchar(30) DEFAULT NULL,
  `lingua` varchar(30) DEFAULT NULL,
  `categ` varchar(200) DEFAULT NULL,
  `scaffale` varchar(30) DEFAULT NULL,
  `quantita` decimal(10,2) DEFAULT NULL,
  `infoeditore` varchar(200) DEFAULT NULL,
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

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2016-01-30  8:24:37
