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

//검색화면에서 필요한 함수들
function SeasonCheckbox (a) {
    var obj = document.getElementsByName("seasonCheck");
    for (var i = 0; i <obj.length; i++) {
        if (obj[i] != a) {
            obj[i].checked = false;
        }
    }
    dynamicData();
}
function SexCheckbox (a) {
    var obj = document.getElementsByName("sexCheck");
    for (var i = 0; i <obj.length; i++) {
        if (obj[i] != a) {
            obj[i].checked = false;
        }
    }
    dynamicData();
}
function CategoryCheckbox (a) {
    var obj = document.getElementsByName("categoryCheck");
    for (var i = 0; i <obj.length; i++) {
        if (obj[i] != a) {
            obj[i].checked = false;
        }
    }
    dynamicData();
}
function PriceCheckbox (a) {
    var obj = document.getElementsByName("priceCheck");
    for (var i = 0; i <obj.length; i++) {
        if (obj[i] != a) {
            obj[i].checked = false;
        }
    }
    dynamicData();
}

//검색에서 쓸 함수들
//체크상태를 확인하면서 배열에서 빼내고 마지막으로 동적객체로 넣자.
//체크 됐을때 함수 부르는것 추가하기 css손보기 
var sortState = "별점순";   //일단 별점 순!

function Body3remove(){
    var body3 = document.getElementById("body3");
    //var parent = body3.parentElement;
    //parent.removeChild(body3);
    while(body3.hasChildNodes()){
        body3.removeChild(body3.firstChild);
    }
}
function dynamicData(){

    //추가로 시도
    Body3remove();  //다 지워주고
    //var body3 = document.createElement("div");  //div객체만들어주고
    //body3.setAttribute("id","body3");
    //var body2 = document.createElement("body2");
    //body2.appendChild(body3);

    var updateArray1 = new Array();
    var updateArray2 = new Array();
    var updateArray3 = new Array();
    var updateArray4 = new Array();

    var seasonObj = document.getElementsByName("seasonCheck"); //계절 어떤것이 체크되어있는지 확인
    var checkedSeason = 0;
    for (var i = 0; i <seasonObj.length; i++) {
        if(seasonObj[i].checked){   
            checkedSeason = seasonObj[i].id;
        }
    }
    if(checkedSeason==0){   //체크된게 없다면
        updateArray1 = perfumeDataArray; //전체를 다 가져감
    }
    else{   //체크된게 있다면
        for(var i = 0;i<perfumeDataArray.length;i++){
            if(perfumeDataArray[i].getSeason() == checkedSeason){   //계절 일치하는 것만 저장
                updateArray1[updateArray1.length] = perfumeDataArray[i];
            }
        }
    }

    var sexObj = document.getElementsByName("sexCheck");    //성별 어떤것이 체크되어있는지
    var checkedSex = 0;
    for(var i = 0; i<sexObj.length;i++){
        if(sexObj[i].checked){
            checkedSex = sexObj[i].id;
        }
    }
    if(checkedSex == 0){    //체크된게 없다면
        updateArray2 = updateArray1; //그전거의 전체를 다 가져감.
    }
    else{   //체크된게 있다면
        for(var i = 0; i<updateArray1.length;i++){
            if(updateArray1[i].getSex() == checkedSex){ //성별 일치하는 것만 저장
                updateArray2[updateArray2.length] = updateArray1[i];
            }
        }
    }

    var categoryObj = document.getElementsByName("categoryCheck");  //계열 어떤것이 체크되어있는지
    var checkedCategory = 0;
    for(var i =0;i<categoryObj.length;i++){
        if(categoryObj[i].checked){
            checkedCategory = categoryObj[i].id;
        }
    }
    if(checkedCategory == 0){   //체크된게 없다면
        updateArray3 = updateArray2;    //그전거의 전체를 다 가져감.
    }
    else{
        for(var i = 0; i<updateArray2.length;i++){  //체크된게 있다면
            if(updateArray2[i].getCategory() == checkedCategory){   //계열 일치하는 것만 저장
                updateArray3[updateArray3.length] = updateArray2[i];
            }
        }
    }

    var priceObj = document.getElementsByName("priceCheck");    //가격대가 어떤것이 체크되어있는지
    var checkedPrice = 0;
    for(var i =0; i<priceObj.length;i++){
        if(priceObj[i].checked){
            checkedPrice = priceObj[i].id;
        }
    }
    if(checkedPrice == 0){  //체크된게 없다면
        updateArray4 = updateArray3;    //그전거의 전체를 가져감
    }
    else{   //체크된게 있다면
        //가격 분별 어케하지...
        for(var i = 0; i<updateArray3.length;i++){
            if(updateArray3[i].getPriceTag() == checkedPrice){  //가격대 일치하는 것만 저장
                updateArray4[updateArray4.length] = updateArray3[i];
            }
        }
    }
    //정렬 기준확인후 배열을 다시 구성하고 그 배열을 함수로 동적 구성하자
    if(sortState =="별점순"){
        //별점순 정렬함수
        starSort(updateArray4);
    }
    else if(sortState =="높은가격순"){
        //높은가격순 정렬함수
        highPriceSort(updateArray4);
    }
    else{
        //낮은가격순 정렬함수
        lowPriceSort(updateArray4);
    }

    createDynamicDIV(updateArray4);
}


//     void bubbleSort(float A[], int n)
// {
// 	float temp; //배열 안의 값을 교환할 때 사용할 변수

// 	for (int i = n - 1; i >= 1; i--) //배열의 마지막 원소부터 1번 원소까지 결정될 것이다.(정렬된 것의 개수를 뜻하게 된다.)
// 	{
// 		for (int j = 0; j <= i - 1; j++) //배열의 0번원소부터 정렬이 완료된 i번원소 전까지 인접한 값을 비교한다.
// 		{
// 			if (A[j] > A[j + 1]) //인접한 원소와 비교하여 더 큰 값을 오른쪽으로 옮긴다.
// 			{
// 				temp = A[j + 1];
// 				A[j + 1] = A[j];
// 				A[j] = temp;
// 			}
// 		}
// 	}
// }
function starSort(array){
    var temp;
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
function highPriceSort(array){
    var temp;
    for(var i = array.length-1; i>=1;i--){
        for(var j = 0; j<=i-1;j++){
            if(array[j].getPrice() < array[j+1].getPrice()){
                temp = array[j+1];
                array[j+1] = array[j];
                array[j] = temp;
            }
        }
    }
}
function lowPriceSort(array){
    var temp;
    for(var i = array.length-1; i>=1;i--){
        for(var j = 0; j<=i-1;j++){
            if(array[j].getPrice() > array[j+1].getPrice()){
                temp = array[j+1];
                array[j+1] = array[j];
                array[j] = temp;
            }
        }
    }
}

function createDynamicDIV(arrayobj){    //최종으로 선택된 것들 동적으로 나타내기
    var updateArray = new Array();
    updateArray = arrayobj;

    //var body2 = document.getElementById("body2");
    var body3 = document.getElementById("body3");
    for(var i = 0; i<updateArray.length; i++){
        var newDiv = document.createElement("div"); //div객체 만들고
        newDiv.setAttribute("class","perfume");
        newDiv.setAttribute("Data",updateArray[i]);
        
        var newImg = document.createElement("img"); //img객체 추가
        newImg.setAttribute("id","perfumeImg");
        newImg.src = updateArray[i].getImage();
        newImg.style.width = "100%";
        newDiv.appendChild(newImg); //향수 이미지 추가
        
        var newSpan = document.createElement("span");
        newSpan.innerHTML = updateArray[i].getName();
        newDiv.appendChild(newSpan);    //향수 이름 추가

        var newSpan2 = document.createElement("span");
        var priceString = updateArray[i].getPrice().toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
        newSpan2.innerHTML = " "+priceString + "원";
        //newSpan2.style.float = "right";
        //newSpan2.innerHTML = updateArray[i].getPrice().toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",");
        //newSpan2.style.float = "right";
        newDiv.appendChild(newSpan2);   //향수 가격 추가

        var newHr = document.createElement("hr");
        newDiv.appendChild(newHr);  //선하나

        //별점시도!
        var newStarDiv = document.createElement("div");
        newStarDiv.setAttribute("id","star");
        newStarDiv.style.display ="inline-block";
        var star = updateArray[i].getStar();    //별점 가져와서
        star = Math.floor(star);    //소수점 떼고
        for(var t =0; t<5;t++){ //총 별점 5개를 표시할건데
            if(t+1<=star){ 
                var starImg = document.createElement("img");
                starImg.src = "media/orangeStar.png";
                starImg.style.width = "14px";
                newStarDiv.appendChild(starImg);
            }
            else{
                var starImg = document.createElement("img");
                starImg.src = "media/orange_blank_star.png";
                starImg.style.width = "14px";
                newStarDiv.appendChild(starImg);
            }
        }
        newDiv.appendChild(newStarDiv);


        var newSpan3 = document.createElement("span");
        newSpan3.innerHTML = "("+updateArray[i].getGradeNum()+")";
        newDiv.appendChild(newSpan3);   //후기 개수 추가
        body3.appendChild(newDiv);

        newDiv.setAttribute("onclick",updateArray[i].getSite());
        //newDiv.onclick = function(this){    //제품 클릭시!
            //location.href=updateArray[i].getSite();
            //경훈님, 이 for문 세번째 줄에 div객체에 Data필드로 해당향수객체 저장해뒀으니까 이걸로 페이지 동적으로 구성하면 될거같아요.. 
        //}
        // <div class="perfume">
        //     <img src="media/chanel.jpg" width="200px" id="perfumeImg" alt="">
        //     <span>perfume1</span><span id="perfumePrice">  106,000원</span>
        //     <hr/>
        //     <div id="star">
        //         <img src="media/star.png" width="14px" alt="">
        //         <img src="media/star.png" width="14px" alt="">
        //         <img src="media/star.png" width="14px" alt="">
        //         <img src="media/star.png" width="14px" alt="">
        //         <img src="media/star.png" width="14px" alt="">
        //     </div>
        //     <span>(15)</span>
        // </div>
    }
}
//정렬함수
function sortClick(obj){
    sortState = obj.id; //정렬 기준값 바꿔준다.
    obj.style.fontWeight = "bold";  //일단 그 객체 볼드체로 바꿔주고
    obj.style.color = "black"
    var sortMode = document.getElementsByClassName("sort"); //다른 객체 다 가져온 다음 다른건 다시 노말로!
    for(var i =0; i<sortMode.length;i++){
        if(sortMode[i].id != obj.id){
            sortMode[i].style.fontWeight = "normal";
        }
    }
    dynamicData();
}
//랭킹향수관리함수
function rankPerfume(){
    var ranked = new Array();
    ranked = perfumeDataArray;
    starSort(ranked);

    var rankedPerfume = document.getElementsByClassName("rank_of_perfume"); //가져옴 세개

    for(var i = 0; i<rankedPerfume.length; i++){
        //rankedPerfume.img.style.src = ranked[i].getImage();
        //rankedPerfume.span.innerHTML = ranked[i].getName();
        rankedPerfume[i].setAttribute("onclick",ranked[i].getSite());
        //newDiv.setAttribute("onclick",updateArray[i].getSite());
        var child = rankedPerfume[i].children;
        child[0].src = ranked[i].getImage();
        child[1].innerHTML = ranked[i].getName();
    }
}
//무시하세요
/* <div class="perfume">
<img src="media/chanel.jpg" width="200px" id="perfumeImg" alt="">
<span>perfume1</span><span>  106,000원</span>
<hr/>
<span>(15)</span>
</div> */
function resultCheck(){
    var season = localStorage.getItem("season"); //계절 로컬에서 가져옴
    var flavor = localStorage.getItem("flavor"); //계열 로컬에서 가져옴
    var gender = localStorage.getItem("gender"); //성별 로컬에서 가져옴
    var price = localStorage.getItem("price");   //가격 로컬에서 가져옴

    if(season!=null && flavor!=null && gender!=null && price!=null){    //뭐가 있으면 이대로 적용
        var SeasonCheckboxAll = document.getElementsByName("seasonCheck");  //계절 반영
        for(var i = 0; i<SeasonCheckboxAll.length;i++){
            if(SeasonCheckboxAll[i].id==season)
            {
                SeasonCheckboxAll[i].checked = true;
            }
        }
        var FlavorCheckboxAll = document.getElementsByName("categoryCheck");
        for(var i = 0; i<FlavorCheckboxAll.length;i++){
            if(FlavorCheckboxAll[i].id==flavor)
            {
                FlavorCheckboxAll[i].checked = true;
            }
        }
        var GenderCheckboxAll = document.getElementsByName("sexCheck");
        for(var i = 0; i<GenderCheckboxAll.length;i++){
            if(GenderCheckboxAll[i].id==gender)
            {
                GenderCheckboxAll[i].checked = true;
            }
        }
        //5,10,20,40,50
        var PriceCheckboxAll = document.getElementsByName("priceCheck");
        for(var i = 0; i<PriceCheckboxAll.length;i++){
            if(PriceCheckboxAll[i].id==(price*10000))
            {
                PriceCheckboxAll[i].checked = true;
            }
        }
        localStorage.removeItem("season");
        localStorage.removeItem("flavor");
        localStorage.removeItem("gender");
        localStorage.removeItem("price");
        dynamicData();
    }
    else{   //없으면 그냥 실행.
        dynamicData();
    }
}
