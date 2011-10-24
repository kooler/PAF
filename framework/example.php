<?php
include_once 'paf/aspect.php';

class Book {
	private $rating;
	
	function __construct() {
		$this->rating = 0;
	}

	public function addRating() {
		$this->rating++;
	}
	
	public function decRating() {
		$this->rating--;
	}
}

class LoggerAspect extends Aspect {
	/**
	 * Log rating incrementation
	 * @After(Book->addRating)
	 */
	function logRatingInc() {
		echo "Rating has been incremented";
	}
	/**
	 * Log rating decrementing
	 * @After(Book->decRating)
	 */
	function logRatingDec() {
		echo "Rating has been decremented";
	}
	/**
	 * Logger tags
	 * @Around(Book->(addRating|decRating))
	 */
	function loggerWrap() {
		echo "|";
	}
	/**
	 * Sho each logger message on new line
	 * @After(Book->(addRating|decRating))
	 */
	function loggerNewLine() {
		echo "\n";
	}
}

AspectRegistry::getInstance()->addAspect(new LoggerAspect);
AspectRegistry::getInstance()->interceptAll();

//Test aspect joinpoints call
$book = new Book();
$book->addRating();
$book->decRating();
$book->addRating();


