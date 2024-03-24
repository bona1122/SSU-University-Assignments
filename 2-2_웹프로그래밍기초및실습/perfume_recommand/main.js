//main.js는 로그인/로그아웃 해주는 자바스크립트

function getCookie(cname) { 
    var name = cname + "=";
    var decodedCookie = decodeURIComponent(document.cookie);
    var ca = decodedCookie.split(';');
    for(var i = 0; i <ca.length; i++) {
        var c = ca[i];
        while (c.charAt(0) == ' ') {
        c = c.substring(1);
        }
        if (c.indexOf(name) == 0) {
        return c.substring(name.length, c.length);
        }
    }
    return "";
}

function deleteCookie( name, path, domain ) {
    if( getCookie( name ) ) {
        document.cookie = name + "=" +
        ((path) ? ";path="+path:"")+
        ((domain)?";domain="+domain:"") +
        ";expires=Thu, 01 Jan 1970 00:00:01 GMT";
    }
}

// 로그인 / 로그아웃 버튼 렌더링
function renderLoginLogoutBtn() {
    var btn = $('#login-logout-btn');
    var user_name = $('#user-name');

    var login_name = getCookie('login_user');
    if(login_name) {
        btn.text('로그아웃');
        user_name.text(login_name + "님 환영합니다.");
    } else {
        btn.text('로그인');
    }
}

renderLoginLogoutBtn();

// 로그인/로그아웃 버튼 눌렸을 때 호출 되는 함수
function onClickLoginLogoutBtn() {
    var login_name = getCookie('login_user');

    if(login_name) {
        var logout = confirm("로그아웃 하시겠습니까?");

        if(logout) {
            alert("로그아웃 되었습니다");
            deleteCookie('login_user');
            location.href = "메인화면.html";
        }

    } else {
        location.href = "로그인.html"
    }
}

$('#login-logout-btn').click(onClickLoginLogoutBtn);

// navigation 눌렸을 때 로그인 상태에 따라 다르게 반응하는 함수
function onClickFindingperfume() {
    var login_name = getCookie('login_user');

    if(login_name) {
        location.href = "#perfume";
    } else {
        alert("로그인을 먼저 해주세요");
        location.href = "로그인.html"
    }
}

$('#pick-link').click(onClickFindingperfume);