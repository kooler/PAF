<?php
include_once 'paf/aspect.php';
/******************************************************************
 **************************** Example *****************************
 ******************************************************************/

class Book {
	private $rating;
	
	function __construct() {
		$this->rating = 0;
	}

	private function addRating() {
		$this->rating++;
	}
	
	private function decRating() {
		$this->rating--;
	}
}

class LoggerAspect extends Aspect {
	/**
	 * Log rating incrementation
	 * @After(Book->addRating)
	 */
	function logRatingInc() {
		echo 'Rating has been incremented';
	}
	/**
	 * Log rating decrementing
	 * @After(Book->decRating)
	 */
	function logRatingDec() {
		echo 'Rating has been decremented';
	}
}

AspectRegistry::getInstance()->addAspect(new LoggerAspect);
AspectRegistry::getInstance()->interceptAll();


