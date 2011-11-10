<?php
include_once 'class_reflection.php';
include_once 'joinpoint.php';
include_once 'annotation_filter.php';
include_once 'aspect_registry.php';
include_once 'interception_chain.php';
/*
 * Base aspect class. Must be extended in all aspects
 */
abstract class Aspect {
	final public function __construct() {}
};
