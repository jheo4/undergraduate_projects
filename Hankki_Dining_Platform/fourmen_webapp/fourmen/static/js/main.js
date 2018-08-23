var autocomplete, marker, map;
var geolocation = {
  locality: 'long_name',
  administrative_area_level_1: 'short_name',
  country: 'short_name',
  lat: document.getElementById("lat"),
  lng: document.getElementById("lng")
}

function init() {
  const input = document.getElementById('search-input');
  const btnLoc = document.getElementById('search-loc');

  btnLoc.addEventListener('click', function (event) {
    event.preventDefault(); // Prevent submit event
    getCurrentGeo();
  });

  map = new google.maps.Map(document.getElementById('map'), {
    center: {lat: 37.566, lng: 126.977},
    zoom: 13
  });

  input.addEventListener('keydown', function (event) {
    if (event.keyCode == '13') {
      event.preventDefault();
    }
  });

  marker = new google.maps.Marker({
    map: map,
    anchorPoint: new google.maps.Point(0, -29)
  });
  autocomplete = new google.maps.places.Autocomplete(
    /** @type {!HTMLInputElement} */ (
        input), {
        types: ['(cities)'],
    });
  autocomplete.addListener('place_changed', fillDataAndSubmit);
}

function fillDataAndSubmit() {
  marker.setVisible(false);
  var place = autocomplete.getPlace();
  for (var entry in geolocation) {
    entry.value = "";
  }
  if (place.address_components == undefined) {
    alert('주소 정보가 올바르지 않습니다');
    document.getElementById("search-input").value = "";
    return;
  }
  for (var i = 0; i < place.address_components.length; i++) {
    var addressType = place.address_components[i].types[0];
    if (geolocation[addressType]) {
      var val = place.address_components[i][geolocation[addressType]];
      document.getElementById(addressType).value = val;
    }
  }

  geolocation.lat.value = place.geometry.location.lat();
  geolocation.lng.value = place.geometry.location.lng();

  if (place.geometry.viewport) {
    map.fitBounds(place.geometry.viewport);
  } else {
    map.setCenter(place.geometry.location);
    map.setZoom(17);  // Why 17? Because it looks good.
  }
  marker.setPosition(place.geometry.location);
  marker.setVisible(true);

  if (mealSearch) {
    mealSearch();
  }
}

function getCurrentGeo() {
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(function (position) {
      var latLng = {
        lat: parseFloat(position.coords.latitude.toPrecision(5)),
        lng: parseFloat(position.coords.longitude.toPrecision(5)),
      };

      var geocoder = new google.maps.Geocoder;
      geocoder.geocode({
        'location': latLng,
      }, function (result, status) {
        if (status == 'OK') {
          let formatted = "";
          for (var i = 0; i < result[0].address_components.length; i++) {
            var addressType = result[0].address_components[i].types[0];
            if (geolocation[addressType]) {
              var val = result[0].address_components[i][geolocation[addressType]];
              document.getElementById(addressType).value = val;
              switch (addressType) {
                case 'locality':
                  formatted += val + ', ';
                  break;
                case 'administrative_area_level_1':
                  formatted += val + ', ';
                  break;
                case 'country':
                  formatted += val;
                  break;
                default:
                  break;
              }
            }
          }
          geolocation.lat.value = latLng.lat;
          geolocation.lng.value = latLng.lng;

          map.setCenter(result[0].geometry.location);
          map.setZoom(15);  // Why 17? Because it looks good.

          marker.setPosition(result[0].geometry.location);
          marker.setVisible(true);
          document.getElementById("search-input").value = formatted;
        }
      });
    });
  }
}
