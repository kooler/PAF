# PHP Aspect framework(PAF)

By default PHP doesn't allow to use Aspects without making wrappers to all methods, that must be intercepted. This approach is not always usable because need a lot of extra code and may cause a lot of problems if have to be implemented on existings systems.

This framework+module allows to use Aspect Oriented Programming without wrappers, but creating Aspects with interceptors to defined object/methods. Interception rules are defined in comments.

To allow method intercepting I made Intercept module that may be found in "intercept_extension" directory.
This module is modification of "PHP Intercept"(I got it here: http://pecl.php.net/package/intercept) with added functionality.

For example, we have object:
<pre><code>class Book {
	public function sayHello() {
		echo 'Hello';
	}
}
</code></pre>
and we want to have interceptor that will insert "!" after each **sayHello()** call. Let's create new aspect with interceptor and define rule for intercepting **Book->seyHello**:
<pre><code>class BookAspect extends Aspect {
	/**
	 * This is our interceptor. In the next line we will define the rule when this method have to be called
	 * @After(Book->sayHello)
	 /
	public function finishTheSentence($params) {
		echo "!\n";
	}
}
</code></pre>
Now all we need to make them all together - register aspect and apply all inteceptors:
<pre><code>AspectRegistry::getInstance()->addAspect(new BookAspect);
AspectRegistry::getInstance()->interceptAll();
</code></pre>

Now after each time when we call **Book->sayHello** the **BookAspect->finishTheSentence** will be called.

Detailed information and examples are here: <https://github.com/kooler/PAF/wiki>