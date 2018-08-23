$('.ui.rating').rating();
$('.ui.rating .icon').click(function(event) {
  let icons = document.getElementsByClassName('icon selected');
  let rating = document.getElementById('rating');
  rating.value = icons.length;
});

