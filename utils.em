/* Utils.em - a small collection of useful editing macros */

/*-------------------------------------------------------------------------*/
macro b_superBackspace()
{
    hwnd = GetCurrentWnd();
    hbuf = GetCurrentBuf();

    if(hbuf == 0)
        stop;   // empty buffer

    // get current cursor postion
    ipos = GetWndSelIchFirst(hwnd);

    // get current line number
    ln = GetBufLnCur(hbuf);

    if((GetBufSelText(hbuf) != "") || (GetWndSelLnFirst(hwnd) != GetWndSelLnLast(hwnd))) {
        // sth. was selected, del selection
        SetBufSelText(hbuf, " ");  // stupid & buggy sourceinsight :(
        // del the " "
        b_superBackspace(1);
        stop;
    }

    // copy current line
    text = GetBufLine(hbuf, ln);

    // get string length
    len = strlen(text);

    // if the cursor is at the start of line, combine with prev line
    if(ipos == 0 || len == 0) {
        if(ln <= 0)
            stop;   // top of file
        ln = ln - 1;    // do not use "ln--" for compatibility with older versions
        prevline = GetBufLine(hbuf, ln);
        prevlen = strlen(prevline);
        // combine two lines
        text = cat(prevline, text);
        // del two lines
        DelBufLine(hbuf, ln);
        DelBufLine(hbuf, ln);
        // insert the combined one
        InsBufLine(hbuf, ln, text);
        // set the cursor position
        SetBufIns(hbuf, ln, prevlen);
        stop;
    }

    num = 1; // del one char
    if(ipos >= 1) {
        // process Chinese character
        i = ipos;
        count = 0;
        while(AsciiFromChar(text[i - 1]) >= 160) {
            i = i - 1;
            count = count + 1;
            if (i == 0)
                break;
        }
        if(count > 0) {
            // I think it might be a two-byte character
            num = 2;
            // This idiot does not support mod and bitwise operators
            if((count / 2 * 2 != count) && (ipos < len))
                ipos = ipos + 1;    // adjust cursor position
        }
    }

    // keeping safe
    if(ipos - num < 0)
        num = ipos;

    // del char(s)
    text = cat(strmid(text, 0, ipos - num), strmid(text, ipos, len));
    DelBufLine(hbuf, ln);
    InsBufLine(hbuf, ln, text);
    SetBufIns(hbuf, ln, ipos - num);
    stop;
}

macro c_insertHeader()//alt+c
{
	hbuf = GetCurrentBuf();
	filename = GetBufName(hbuf);

	pbuf = GetCurrentProj();
	porjname = GetProjName(pbuf);

	szTime  = GetSysTime(1);
	year = szTime.year
	month = szTime.month
	day = szTime.day
	time = szTime.time

	InsBufLine(hbuf, 0,  "/**")
	InsBufLine(hbuf, 1,  "  ********************************************************************************")
	InsBufLine(hbuf, 2,  "  * @@File Name    : @filename@")
	InsBufLine(hbuf, 3,  "  * @@Author       : Jungle")
	InsBufLine(hbuf, 4,  "  * @@Mail         : Mail")
	InsBufLine(hbuf, 5,  "  * @@Created Time : @year@/@month@/@day@ @time@")
	InsBufLine(hbuf, 6,  "  * @@Version      : V1.0")
	InsBufLine(hbuf, 7,  "  * @@Last Changed : @year@/@month@/@day@ @time@")
	InsBufLine(hbuf, 8,  "  * @@Brief        : brief")
	InsBufLine(hbuf, 9,  "  ********************************************************************************")
	InsBufLine(hbuf, 10, "  */")
	InsBufLine(hbuf, 11, "")
	InsBufLine(hbuf, 12, "/* Inlcude ---------------------------------------------------------------------*/")
	InsBufLine(hbuf, 13, "")
	InsBufLine(hbuf, 14, "/** @@addtogroup @porjname@_Project")
	InsBufLine(hbuf, 15, "  * @@{")
	InsBufLine(hbuf, 16, "  */")
	InsBufLine(hbuf, 17, "")
	InsBufLine(hbuf, 18, "/* Private typedef -------------------------------------------------------------*/")
	InsBufLine(hbuf, 19, "/* Private constants define ----------------------------------------------------*/")
	InsBufLine(hbuf, 20, "/* Private macro define --------------------------------------------------------*/")
	InsBufLine(hbuf, 21, "/* Private variables -----------------------------------------------------------*/")
	InsBufLine(hbuf, 22, "/* Private function declaration ------------------------------------------------*/")
	InsBufLine(hbuf, 23, "/* Private functions -----------------------------------------------------------*/")
	InsBufLine(hbuf, 24, "/**")
	InsBufLine(hbuf, 25, "  * @@brief  :")
	InsBufLine(hbuf, 26, "  * @@param  : None")
	InsBufLine(hbuf, 27, "  * @@retval : None")
	InsBufLine(hbuf, 28, "  */")
	InsBufLine(hbuf, 29, "void function(void)")
	InsBufLine(hbuf, 30, "{")
	InsBufLine(hbuf, 31, "")
	InsBufLine(hbuf, 32, "}")
	InsBufLine(hbuf, 33, "")
	InsBufLine(hbuf, 34, "/**")
	InsBufLine(hbuf, 35, "  * @@}")
	InsBufLine(hbuf, 36, "  */")
	InsBufLine(hbuf, 37, "")
	InsBufLine(hbuf, 38, "/************************** Coopyright (C) Jungleeee 2018 *******END OF FILE*******/")
}

macro h_insertHeader()//alt+h
{
	hbuf = GetCurrentBuf();
	filename = GetBufName(hbuf);
	cch = strlen(filename);
	HEADER = toupper(strtrunc(filename, cch - 2));

	szTime  = GetSysTime(1);
	year = szTime.year
	month = szTime.month
	day = szTime.day
	time = szTime.time

	InsBufLine(hbuf, 0,  "/**")
	InsBufLine(hbuf, 1,  "  ********************************************************************************")
	InsBufLine(hbuf, 2,  "  * @@File Name    : @filename@")
	InsBufLine(hbuf, 3,  "  * @@Author       : Jungle")
	InsBufLine(hbuf, 4,  "  * @@Mail         : Mail")
	InsBufLine(hbuf, 5,  "  * @@Created Time : @year@/@month@/@day@ @time@")
	InsBufLine(hbuf, 6,  "  * @@Version      : V1.0")
	InsBufLine(hbuf, 7,  "  * @@Last Changed : @year@/@month@/@day@ @time@")
	InsBufLine(hbuf, 8,  "  * @@Brief        : brief")
	InsBufLine(hbuf, 9,  "  ********************************************************************************")
	InsBufLine(hbuf, 10, "  */")
	InsBufLine(hbuf, 11, "")
	InsBufLine(hbuf, 12, "/* Define to prevent recursive inclusion ---------------------------------------*/")
	InsBufLine(hbuf, 13, "#ifndef __@HEADER@_H_")
	InsBufLine(hbuf, 14, "#define __@HEADER@_H_")
	InsBufLine(hbuf, 15, "")
	InsBufLine(hbuf, 16, "/* Inlcude ---------------------------------------------------------------------*/")
	InsBufLine(hbuf, 17, "")
	InsBufLine(hbuf, 18, "/* Exported typedef ------------------------------------------------------------*/")
	InsBufLine(hbuf, 19, "/* Exported constants define ---------------------------------------------------*/")
	InsBufLine(hbuf, 20, "/* Exported macro define -------------------------------------------------------*/")
	InsBufLine(hbuf, 21, "/* Exported variables ----------------------------------------------------------*/")
	InsBufLine(hbuf, 22, "/* Exported functions ----------------------------------------------------------*/")
	InsBufLine(hbuf, 23, "")
	InsBufLine(hbuf, 24, "#endif /* __@HEADER@_H_ */")
	InsBufLine(hbuf, 25, "")
	InsBufLine(hbuf, 26, "/************************** Coopyright (C) Jungleeee 2018 *******END OF FILE*******/")
}

macro s_saveFileUpdateLastChangeTime()//alt+s
{
	hbuf = GetCurrentBuf();
	szS = GetBufLine(hbuf, 7)

	if(strmid(szS, 0, 20) == "  * @@Last Changed : ") {
		szTime  = GetSysTime(1)
		year = szTime.year
		month = szTime.month
		day = szTime.day
		time = szTime.time

		PutBufLine(hbuf, 7,  "  * @@Last Changed : @year@/@month@/@day@ @time@")
	}

	SaveBuf(hbuf);
}

macro n_multiLineComment()//alt+n
{
    hwnd = GetCurrentWnd();
    selection = GetWndSel(hwnd);
    lnFirst =GetWndSellnFirst(hwnd);      	//取首行行号
    lnLast =GetWndSellnLast(hwnd);      	//取末行行号
    hbuf = GetCurrentBuf();

    if(GetBufLine(hbuf, 0) =="//magic-number:tph85666031"){
        stop
    }

    ln = lnfirst;
    buf = GetBufLine(hbuf, ln);
    len = strlen(buf);

    while(ln <= lnlast) {
        buf = GetBufLine(hbuf, ln);			//取Ln对应的行
        if(buf =="") {						//跳过空行
            ln = ln + 1;
            continue;
        }

        if(strmid(buf, 0, 1) == "/"){       //需要取消注释,防止只有单字符的行
            if(strmid(buf, 1, 2) == "/"){
                PutBufLine(hbuf, ln, strmid(buf, 2, Strlen(buf)));
            }
        }

        if(strmid(buf, 0, 1) != "/"){          //需要添加注释
            PutBufLine(hbuf, ln, Cat("//", buf));
        }
        ln = ln + 1;
    }

    SetWndSel(hwnd, selection);
}

macro d_macroComment()//alt+d
{
    hwnd = GetCurrentWnd();
    sel = GetWndSel(hwnd);

    lnFirst = GetWndSelLnFirst(hwnd);
    lnLast = GetWndSelLnLast(hwnd);
    hbuf = GetCurrentBuf();

    if(lnFirst == 0) {
		szIfStart = "";
    }
    else {
		szIfStart = GetBufLine(hbuf, lnFirst - 1);
    }

    szIfEnd = GetBufLine(hbuf, (lnLast + 1));
    if((szIfStart == "#if 0") && (szIfEnd == "#endif")) {
		DelBufLine(hbuf, lnLast + 1);
		DelBufLine(hbuf, lnFirst - 1);
		sel.lnFirst = sel.lnFirst - 1;
		sel.lnLast = sel.lnLast - 1;
    }
    else {
		InsBufLine(hbuf, lnFirst, "#if 0");
		InsBufLine(hbuf, (lnLast + 2), "#endif");
		sel.lnFirst = sel.lnFirst + 1;
		sel.lnLast = sel.lnLast + 1;
    }

    SetWndSel(hwnd, sel);
}
