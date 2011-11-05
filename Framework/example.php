<?php
include_once 'paf/aspect.php';

class Book {
	private $rating;
	public $name;
	
	function __construct() {
		$this->rating = 0;
	}

	public function addRating($lala) {
		$this->rating++;
	}
	
	public function decRating($lala) {
		$this->rating--;
	}
}

class LoggerAspect extends Aspect {
	/**
	 * Logger tags
	 * @Around(Book->(addRating|decRating))
	 */
	function loggerWrap() {
		echo "|";
	}
	/**
	 * Log rating incrementation
	 * @Before(Book->addRating)
	 */
	function logRatingInc($params) {
		echo "Rating for book '{$params[0]->name}' has been incremented with message '{$params[1][0]}'";
	}
	/**
	 * Log rating decrementing
	 * @After(Book->decRating)
	 */
	function logRatingDec($params) {
		echo "Rating for book '{$params[0]->name}' has been decremented with message '{$params[1][0]}'";
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
$book->name = 'My test book';
$book->addRating("hello");
$book->decRating("buy");

