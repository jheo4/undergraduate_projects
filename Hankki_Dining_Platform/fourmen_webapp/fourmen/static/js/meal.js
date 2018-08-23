$('#meal_date').datepicker({
  dateFormat: 'yy-mm-dd',
});
$('#meal_time').timepicker({
  'scrollDefault': 'now',
  'step': 30,
  'timeFormat': 'H:i',
  'minTime': '8:00',
  'maxTime': '22:00',
});
