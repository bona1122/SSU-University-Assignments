function PerfumeData(name,price,img,sex,season,category,star,gradeNum,priceTag,site){ //향수 객체
    this.name = name;   //이름
    this.price = price; //가격
    this.img = img; //이미지
    this.sex = sex; //성별
    this.season = season;   //계절
    this.category = category;   //계열
    this.star = star; //별점
    this.gradeNum = gradeNum; //평점 개수
    this.priceTag = priceTag;   //가격대(이시영만 씀)
    this.site = site;   //향수 상세페이지 url

    this.getName = function(){return this.name;}    //이름 알아내기
    this.getPrice = function(){return this.price;}  //가격 알아내기
    this.getImage = function(){return this.img;}    //이미지 알아내기
    this.getSex = function(){return this.sex;}  //성별 알아내기
    this.getSeason = function(){return this.season;}    //계절 알아내기
    this.getCategory = function(){return this.category;}    //계열 알아내기
    this.getStar = function(){return this.star;}    //별점 알아내기
    this.getGradeNum = function(){return this.gradeNum;}    //평점 개수 알아내기
    this.getPriceTag = function(){return this.priceTag;}    //가격대 알아내기(이시영만 씀)
    this.getSite = function(){return this.site;}    //상세페이지 url알아내기

    this.addStar = function(plusStar){  //별점등록하면 평점개수 올라가고 별점평균(평균 방식 생각해야될듯)
        this.gradeNum++;
        this.star = (this.star + plusStar) / 2;}
}

var perfumeDataArray = new Array(); //향수객체 배열

function plusData(name,price,img,sex,season,category,star,gradeNum,priceTag,site){    //향수 데이터 추가 함수
    var newPerfume = new PerfumeData(name,price,img,sex,season,category,star,gradeNum,priceTag,site);
    perfumeDataArray[perfumeDataArray.length] = newPerfume;
}

//데이터 여기다가 추가해주면 자동으로 객체만들어서 배열에 추가됨
plusData("디올 어딕트 오 프레쉬 오드뚜왈렛(50ml)",124000,"media/dior.jpg","여","여름","시트러스",4.5,17,"200000","location.href='상세정보(디올).html'");
plusData("조말론 우드 세이지 앤 씨 솔트 코롱(100ml)",190000,"media/joMalone.jpg","여","여름","시트러스",3,19,"200000","location.href='상세정보(조말론우드).html'");
plusData("샤넬 No.5 오드퍼퓸(100ml)",176000,"media/chanel.jpg","여","여름","시트러스",5,30,"200000","location.href='상세정보(샤넬).html'");

plusData("조말론런던 오렌지 블로썸 코롱(30ml)",95000,"media/joMalone_Orange_Blossom.jpg","여","봄","플로럴",2,30,"100000","location.href='상세정보(조말론오렌지).html'");
plusData("조말론 레드 로즈 코롱(30ml)",94000,"media/jomalone_red_rose.jpg","여","봄","플로럴",3.5,30,"100000","location.href='상세정보(조말론레드).html'");
plusData("입생로랑 리브르 오드퍼퓸(30ml)",86000,"media/YvesSaintLaurent_lived_Odd.jpg","여","봄","플로럴",4.8,30,"100000","location.href='상세정보(입생로랑).html'");

//검색에서 쓸 함수들
//체크상태를 확인하면서 배열에서 빼내고 마지막으로 동적객체로 넣자.
//체크 됐을때 함수 부르는것 추가하기 css손보기 
function dynamicData(){
    var updateArray1 = new Array();
    //var updateArray2 = new Array();
    //var updateArray3 = new Array();
    //var updateArray4 = new Array();

    updateArray1=perfumeDataArray;

    var seasonObj = localStorage.getItem("season"); //계절 로컬에서 가져옴
    for(var i = 0;i<perfumeDataArray.length;i++){
        if(perfumeDataArray[i].getSeason() != seasonObj){ 
            updateArray1[i] = 0;
        }
    }
    var flavorObj = localStorage.getItem("flavor");
    for(var i = 0;i<perfumeDataArray.length;i++){
        if(perfumeDataArray[i]!=0){
            if(perfumeDataArray[i].getCategory() != flavorObj){
                updateArray1[i] = 0;
            }
        }
    }
    var genderObj = localStorage.getItem("gender");
    for(var i = 0;i<perfumeDataArray.length;i++){
        if(perfumeDataArray[i]!=0){
            if(perfumeDataArray[i].getSex() != genderObj){
                updateArray1[i] = 0;
            }
        }
    }
    var priceObj = localStorage.getItem("price");
    for(var i = 0;i<perfumeDataArray.length;i++){
        if(perfumeDataArray[i]!=0){
            if(perfumeDataArray[i].getPriceTag() != (priceObj*10000)){
                updateArray1[i] = 0;
            }
        }
    }
    starSort(updateArray1);
    createDynamicDIV(updateArray1);
}
function createDynamicDIV(arrayobj){    //최종으로 선택된 것들 동적으로 나타내기
    var updateArray = new Array();
    updateArray = arrayobj;

    var body2 = document.getElementById("body2");
    for(var i = 0; i<updateArray.length; i++){
        if(updateArray[i]==0) {
            continue;
        }
        var newDiv = document.createElement("div"); //div객체 만들고
        newDiv.setAttribute("class","perfume");
        newDiv.setAttribute("Data",updateArray[i]);
        
        var newImg = document.createElement("img"); //img객체 추가
        newImg.setAttribute("id","perfumeImg");
        newImg.src = updateArray[i].getImage();
        newImg.style.width = "200px";
        newDiv.appendChild(newImg); //향수 이미지 추가
        
        var newSpan = document.createElement("span");
        newSpan.innerHTML = updateArray[i].getName();
        newDiv.appendChild(newSpan);    //향수 이름 추가
        
        //상세페이지 연결 시도
        newDiv.setAttribute("onclick",updateArray[i].getSite());

        body2.appendChild(newDiv);
    }
}
function starSort(array){
    var temp;
    var newArray = new Array(); //옮길 배열
    for(var i = 0; i<array.length;i++){
        if(array[i]!=0){
            newArray[newArray.length] = array[i];   //0아닌 것만 저장
        }
    }
    array = newArray;
    for(var i = array.length-1; i>=1;i--){
        for(var j = 0; j<=i-1;j++){
            if(array[j].getStar() < array[j+1].getStar()){
                temp = array[j+1];
                array[j+1] = array[j];
                array[j] = temp;
            }
        }
    }
}