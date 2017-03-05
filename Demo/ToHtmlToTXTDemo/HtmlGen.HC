/* This converts [C:/Demo/ToHtmlToTXTDemo/DemoInPage.DD.Z,1] to
an html document named "OutPage.html".

Notice that an entry like $TX,"GOOGLE",HTML="http://www.google.com"$
will be converted to text in the html with an html link.

I cheated by hardcoding [C:/Demo/ToHtmlToTXTDemo/ToHtml.HC.Z,1] www.templeos.org as the website
for [C:/Adam/DolDoc/DocLink.HC.Z,29] TempleOS Links.  Why don't you copy
[C:/Demo/ToHtmlToTXTDemo/ToHtml.HC.Z,1] to your /Home directory
and modify it?	You are welcome to link to
http://www.templeos.org if you want file that come on the
TempleOS distribution.

You can pass html meta data as args to [C:/Demo/ToHtmlToTXTDemo/ToHtml.HC.Z,1] ToHtml().

If you want to blog like I do, see these files...
[C:/Demo/AcctExample/TOS/TOSToHtml.HC.Z,1]
[C:/Demo/AcctExample/HomeKeyPlugIns.HC.Z,1]
[C:/Demo/AcctExample/TOS/TOSHolySpirit.HC.Z,1]
*/

Cd(__DIR__);;
#include "ToHtml"

ToHtml("DemoInPage.DD.Z","~/DemoOutPage");
