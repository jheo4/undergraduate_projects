function preview(event) {
  var preview = document.getElementById('preview');
  preview.src = URL.createObjectURL(event.target.files[0]);
}


function upload_file(file, callback) {
  $.post("/file_server/put_presigned_url").done(
    function (data) {
      var url = data.presigned_url;
      var object_uuid = data.object_uuid;
      var xhr = new XMLHttpRequest();
      xhr.open('PUT', url, true);
      xhr.send(file);
      xhr.onload = () => {
        if (xhr.status == 200) {
          callback(url, object_uuid);
        }
      };
    }
  );
}

function multiple_preview(event, preview_place){
  var images = event.target.files;

  if(images){
    $(preview_place).empty();
    var num_files = images.length;

    for(i=0; i < num_files; i++){
      var reader = new FileReader();
      reader.onload = function(event){
        $(preview_place).append(`
          <div class="ui image">
            <img src="${event.target.result}" style="max-width: 250px; max-height: 350px;">
          </div>
        `);
      }
      $(preview_place).append('<input type="hidden" name="image_uuids" index="'+i+'">');
      reader.readAsDataURL(images[i]);
    }
  }
}
