function mealSearch() {
  var keyword = $('#keyword').val();
  var lat = $('#lat').val();
  var lng = $('#lng').val();
  var distance = $('#distance option:selected').val();

  if(distance != '' && keyword ==''){
    alert('키워드 입력');
    return;
  }
  if (keyword == '' && !(lat == '' && lng == '')) {
    alert('키워드 입력하세요');
    return;
  }
  if (!keyword == '' && (lat == '' && lng == '')) {
    alert('위치 정보를 입력하세요');
    return;
  }

  $(document).ready(function () {
    $.ajax({
      url: "/meals/search",
      type: 'GET',
      data: {
        keyword: keyword,
        distance: distance,
        lat: lat,
        lng: lng
      },
      contentType: 'application/json; charset=UTF-8',

      success: function (response) {
        $("#meal_list").html(response);
      },
      error: function (error) {
        console.log('ajax call error', error);
      }
    });
  });
}
