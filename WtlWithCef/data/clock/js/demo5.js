let myDate = new Date,
    hours = myDate.getHours(),
    minutes = myDate.getMinutes(),
    seconds = myDate.getSeconds(),
    secondStep = -6 * seconds;

function rotateSecond() {
    setInterval(showDegSecond, 1e3)
}

function secondHtml() {
    let e = "";
    for (let t = 0; t < 60; t++) e += "<li><span>" + NumberToChinese(t) + "</span></li>";
    $(".second ul").html(e);
    for (let e = 0; e < 60; e++) {
        if ($(".second ul li").eq(e).css("transform", "rotate(" + 6 * e + "deg)"), 30 != e) {
            let t = Number(250 * Math.sin(2 * Math.PI / 60 * e)),
                s = Number(250 * Math.cos(2 * Math.PI / 60 * e));
            $(".second ul li").eq(e).css({
                left: 240 + t + "px",
                top: 250 - s + "px"
            })
        } else if (30 == e) {
            let t = Number(250 * Math.sin(2 * Math.PI / 60 * e)),
                s = Number(250 * Math.cos(2 * Math.PI / 60 * e));
            $(".second ul li").eq(e).css({
                left: 240 + t + "px",
                bottom: 250 + s - 60 + "px"
            })
        }
        $(".second ul").css("transform", "rotate(" + -6 * seconds + "deg)")
    }
    minuteHtml()
}

function minuteHtml() {
    let e = "";
    for (let t = 59; t >= 0; t--) e += "<li><span>" + NumberToChinese(t) + "</span></li>";
    $(".minute ul").html(e);
    for (let e = 0; e < 60; e++) {
        if ($(".minute ul li").eq(e).css("transform", "rotate(" + 6 * e + "deg)"), 30 != e) {
            let t = Number(190 * Math.sin(2 * Math.PI / 60 * e)),
                s = Number(190 * Math.cos(2 * Math.PI / 60 * e));
            $(".minute ul li").eq(e).css({
                left: 180 + t + "px",
                top: 190 - s + "px"
            })
        } else if (30 == e) {
            let t = Number(190 * Math.sin(2 * Math.PI / 60 * e)),
                s = Number(190 * Math.cos(2 * Math.PI / 60 * e));
            $(".minute ul li").eq(e).css({
                left: 180 + t + "px",
                bottom: 190 + s - 60 + "px"
            })
        }
        $(".minute").css("transform", "rotate(" + -6 * (59 - minutes) + "deg)")
    }
    hourHtml()
}

function hourHtml() {
    let e = "";
    for (let t = 0; t <= 12; t++) e += "<li><span>" + (t == "0" ? "一十二" : NumberToChinese(t)) + "时</span></li>";
    $(".hour ul").html(e);
    for (let e = 0; e < 12; e++) {
        if ($(".hour ul li").eq(e).css("transform", "rotate(" + 30 * e + "deg)"), 6 != e) {
            let t = Number(130 * Math.sin(2 * Math.PI / 12 * e)),
                s = Number(130 * Math.cos(2 * Math.PI / 12 * e));
            $(".hour ul li").eq(e).css({
                left: 120 + t + "px",
                top: 130 - s + "px"
            })
        } else if (6 == e) {
            let t = Number(130 * Math.sin(2 * Math.PI / 12 * e)),
                s = Number(130 * Math.cos(2 * Math.PI / 12 * e));
            $(".hour ul li").eq(e).css({
                left: 120 + t + "px",
                bottom: 130 + s - 60 + "px"
            })
        }
        $(".hour").css("transform", "rotate(" + 30 * (12 - hours) + "deg)")
    }
    rotateSecond()
}

function showDegSecond() {
    let e = (secondStep -= 6) - 6;
    seconds++, $(".second ul").rotate({
        animateTo: e,
        duration: 1e3
    }), -secondStep % 360 == 0 && (minutes++, $(".minute").rotate({
        animateTo: -6 * (60 - minutes % 60) + 6,
        duration: 1e3
    }), minutes % 60 == 0 && (hours++, $(".hour").css("transform", "rotate(" + 30 * (12 - hours) + "deg)")))
}