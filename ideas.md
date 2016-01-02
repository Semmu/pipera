# Pipera ötletláda

**Megválaszolatlan kérdések:**

- milyen események lesznek, miknek és hogyan lehet azokra feliratkozni? hover, click, drag, resize, keypress?
- hogy lesz a drag'n'drop?????
- failsafe dirty mark kéne: widgetek render()-t definiálják felül, de lesz nem virtual doRender() vagy hasonló, ami render()-t hív, de dirty bitet is (ki)állít
- hogy lesz a resize??? értesíteni kell az érintetteket, új surface-t kell csinálni, stb.
- kurzorok? hogy lesznek megadva, hogy egyáltalán mik vannak? és hogy melyik widget-nek mi a kurzora hover-re???
- több egymásba ágyazott draggable elemnél csak az egyiknek kéne elkapnia az eseményt
- bitmap font??????
- textinput inputbevitel?????????
- az output végül egy AbsoluteContainer? azon meg window-k? vagy szimpla widget-ek? mivel több egy window a widget-nél? (pl hidden bit)
- hotkey support?
- témázhatóság? stílusinformáció elválasztása? hogy ne legyen hardcode-olt minden kinézet
- szülő widget pointer? sokszor kell, pl markDirty
- a hidden jelző lehetne a widget-é a window helyett
- widget globális AABB lekérdezés? ez para, mert az összes szülőjétől függ, főleg, ha még scrollable containerben is van...
- window typeok???
- cursorclass? mármint amihez lehez igazítani n shit

# Milestones

Nagyobb feature-ök, amiket implementálni kell, de nem feltétlenül ilyen sorrendben:

- összes konténer
- bitmap font
- resize
- drag events
- textinput
- widgettől meg eseménytől függő kurzorok
- témázhatóság


### render pipeline

- belépési pont: az Output-on hívódik meg (ami ugye egy AbsoluteContainer)
- ha nem dirty, akkor kész, csak blit. de ha nem:
- bejár child widget-ek, ami dirty, arra doRender()
- blit mindent megfelelő helyre
- kész?


### lazy rendering

- minden tartalmaz saját surface-t
- a render() arra rajzol
- a doRender() meghívja a render(), ha kell (mert dirty), és kiállítja, hogy dirty


### mark dirty

- jelzi, hogy újrarender kell (kb mint a glutPostRedisplay)
- beállítja a dirty bitet
- ha van parent, akkor azon is -> így felfele a gyökérig (output) minden dirty lesz, aminek tényleg újrarender kell


# Konténerek

A widgetek rendezésére meg a normális UI kialakításához kell sok különböző fajta konténer.

Egyelőre nem sikerült egységes APIt kitalálnom az összesnek, így nincs *túl sok* értelme a közös ősnek, de ettől függetlenül lesz. Csak mindegyik teljesen másképp működik.

FYI a widgetek nem tartalmaznak pozícióinformációt magukról, mert az nem mindig releváns (és nagyon konténerfüggő). Így ilyet csak a konténerek tartalmaznak különböző formában.


## AbsoluteContainer

Fix méretű vászon, amire a widgeteket abszolút pozíciók megadásával lehet felvinni és elhelyezni.

Problémák:

- itt jó lenne, ha lehetne használni a PixelPicker-es igazítást, a konténeren belüli widgetek egymáshoz és a konténer egészéhez való igazításához
    - bár ez lehet túl powerful, mert így gyakorlatilag az összes többi konténert implementálni lehet, csak picit körülményesebben
- API?
    - a widget.setPos(X, Y) nem megvalósítható
    - container.setWidgetPos(w, X, Y) meg nem túl szép
    - de ha a widgetek nem tartalmaznak pozíciót, csak a konténerek a bennük levő widgetekről, akkor nincs más megoldás
    - container.alignWidgetTo(w, PP, target, PP)?
    - konténer egészéhez való igazítás: target == null
    - zOrder????


## RowContainer

Widgetek vízszintes egymás után rakásához. Gyakorlatilag egy táblázat, aminek egy sora van, és annyi oszlopa, ahány benne lévő widget. "Cellán" belül 3 igazítási irány: fent, közép, lent.

Problémák:

- API?
    - c.addWidget(w, alignment = center, position = last)
    - c.alignWidget(w, alignment)?
    - widget csere?
    - runtime hozzáadásnál a mérete nő -> szülőt hogy értesít? vagy mi lesz?


## ColumnContainer

Ugyanaz, mint a Row, csak függőlegesen. Igazítás: bal, közép, jobb. problémák is ugyanazok.


## TableContainer

Elég beszédes: táblázat. Így biztosított a helyes igazítás (cellák keretei egy vonalba esnek). Cellán belül 9 igazítási lehetőség.

*Nagyon* sok probléma... Hozzáadás hogy? Widget csere? Fix vagy teljesen dinamikus méret? Beéptett cellamargó? Feltöltés sorban? De ahhoz kell fix méret. Cella kihagyás?


## ScrollableContainer

Nagy tartalom megjelenítése kis területen görgetősávokkal.

- megadható, hogy mely oldalakon legyen scrollbar
- kell hozzá drag'n'move support! hogy a csúszkákat és/vagy tartalmat lehessen húzni.
- scrollbar eltűnik vagy inaktív?
- kicsi child esetén az merre igazodik?


## PaddingContainer

Widget körüli üres hely kihagyásához. Egy child widget-tel rendelkezik + 4 számmal, ami az egyes oldalakon lévő padding. A surface végül child surface + padding méretű.

Problémák:

- nem túl jó, mert fenntartunk egy surface-t, aminek a tartalma teljes egészében a child widgeté, egy kis üres hellyel megspékelve
- szóval redundáns és nem túl optimális


## DecoratedContainer

Mint a PaddingContainer, csak nem teljesen transzparens: tartalmaz egy képet is, ami alapján keretet is rajzol. A padding alapján lecsípi a kép sarkait, amit beilleszt a surface sarkaiba, és a kereteket meg az alap hátteret is megrajzolja a kép megfelelő részeinek ismétlésével.

Probléma:

- redraw-nál elég csak a közepét, a tartalmi részt újratapétázni + blittelni a child-t