from flask import Blueprint, request, \
    url_for, render_template, flash, redirect

from fourmen.models.database import db_session
from fourmen.models.host_application import HostApplication

from . import login_required

mod = Blueprint('admin', __name__, url_prefix='/admin')


@mod.route('/applicants', methods=['GET'])
@login_required
def list_applicants():
    if request.method == 'GET':
        applications = HostApplication.query.filter_by(check=False)
        return render_template('admin_list_applicants.html',
                               applications=applications)


@mod.route('/applicants/<int:application_id>', methods=['GET', 'POST'])
@login_required
def confirm_applicants(application_id):
    application = HostApplication.query.get(application_id)
    if request.method == 'GET':
        return render_template('admin_show_applicant.html',
                               application=application)
    elif request.method == 'POST':
        user = application.user
        application.check = True
        if "confirm" in request.form:
            user.is_host = True
            flash('The '+user.username+'s applicant is confirmed.')
        elif "deny" in request.form:
            flash('The '+user.username+'s applicant is rejected.')
        user.description = request.form['description']
        db_session.add(user)
        db_session.add(application)
        db_session.flush()
        return redirect(url_for('admin.list_applicants'))
