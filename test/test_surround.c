/* 
 * test the surround plugin
 */

int main(int argc, char *argv[]) {
  /*
   * Old text                  Command     New text
   * "Hello *world!"           ds"         Hello world!
   * [123+4*56]/2              cs])        (123+456)/2
   * "Look ma, I'm *HTML!"     cs"<q>      <q>Look ma, I'm HTML!</q>
   * if *x>3 {                 ysW(        if ( x>3 ) {
   * my $str = *whee!;         vllllS'     my $str = 'whee!';
   *
   * gS{
   * if I have this code:
   * if (x>3) *doThing();
   * Then v$ selects the rest of the line
   * then we can gS} to get
   * if (x>3) {
   *    doThing();
   * }
   *
   * ds = delete surrounding
   * cs = change surrounding
   * ys = add surrounding
   * vS = surround visual selection
   *
   * ys[vim motion or text object]: ysiw) you surround inner word )
   * yss = current line minus white space
   *
   * *a/plugin, ys', 'a/plugin'
   *
   */
    // change surrounding cs
      
    // Old text                  Command     New text
    // "Hello *world!"           cs"'        'Hello world!'
    // "Hello *world!"           cs"<q>      <q>Hello world!</q>
    // (123+4*56)/2              cs)]        [123+456]/2
    // (123+4*56)/2              cs)[        [ 123+456 ]/2
    // <div>Yo!*</div>           cst<p>      <p>Yo!</p>

    // delete surrounding ds
    // Old text                  Command     New text
    // "Hello *world!"           ds"         Hello world!
    // (123+4*56)/2              ds)         123+456/2
    // <div>Yo!*</div>           dst         Yo!
    //
    // *ys* takes a valid Vim motion or text object as the first object, and wraps
    // it using the second argument as with |cs|.  (It's a stretch, but a good
    // mnemonic for "ys" is "you surround".)
    //
    //   Old text                  Command     New text
    //   Hello w*orld!             ysiw)       Hello (world)!
    // There is also *yS* and *ySS* which indent the surrounded text and place it
    // on a line of its own.
    //
    // In visual mode, a simple "S" with an argument wraps the selection.  This is
    // referred to as the *vS* mapping, although ordinarily there will be
    // additional keystrokes between the v and S.  In linewise visual mode, the
    // surroundings are placed on separate lines and indented.  In blockwise visual
    // mode, each line is surrounded.
    //
    // A "gS" in visual mode, known as *vgS* , behaves similarly.  In linewise visual
    // mode, the automatic indenting is suppressed.  In blockwise visual mode, this
    // enables surrounding past the end of the line with 'virtualedit' set (there
    // seems to be no way in Vim Script to differentiate between a jagged end of line
    // selection and a virtual block selected past the end of the line, so two maps
    // were needed).


    // As a special case, *yss* operates on the current line, ignoring leading
    // whitespace.
    //
    //   Old text                  Command     New text
    //       Hello w*orld!         yssB            {Hello world!}

    // If f, F, or <C-F> is used, Vim prompts for a function name to insert.  The target
    // text will be wrapped in a function call. If f is used, the text is wrapped with
    // () parentheses; F adds additional spaces inside the parentheses. <C-F> inserts the
    // function name inside the parentheses.
    //
    //   Old text                  Command           New text
    //   "hello"                   ysWfprint<cr>     print("hello")
    //   "hello"                   ysWFprint<cr>     print( "hello" )
    //   "hello"                   ysW<C-f>print<cr> (print "hello")

    // "Hello world!";
    // [123+456]/2;
    // "Look ma, I'm HTML!";
    // if ( x>3 ) {
    //     my $str = 'whee!';
    // }
    return 0;
}
