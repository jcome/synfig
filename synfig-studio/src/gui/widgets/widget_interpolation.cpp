/* === S Y N F I G ========================================================= */
/*!	\file widget_interpolation.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**  Copyright (c) 2008 Chris Moore
**  Copyright (c) 2008, 2011, 2012 Carlos López
**  Copyright (c) 2013 Yu Chen
**
**	This package is free software; you can redistribute it and/or
**	modify it under the terms of the GNU General Public License as
**	published by the Free Software Foundation; either version 2 of
**	the License, or (at your option) any later version.
**
**	This package is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
**	General Public License for more details.
**	\endlegal
*/
/* ========================================================================= */

/* === H E A D E R S ======================================================= */

#ifdef USING_PCH
#	include "pch.h"
#else
#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "widgets/widget_interpolation.h"
#include "app.h"
#include <gtkmm/menu.h>
#include <gtkmm/scale.h>
#include <synfig/exception.h>
#include <synfigapp/main.h>
#include "canvasview.h"
#include "widgets/widget_enum.h"
#include "widgets/widget_distance.h"

#include "general.h"

#endif

/* === U S I N G =========================================================== */

using namespace std;
using namespace etl;
using namespace synfig;
using namespace studio;

/* === M A C R O S ========================================================= */

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Widget_Interpolation::Widget_Interpolation()
{
	//set_size_request(48,48+GRADIENT_HEIGHT+16);
	//set_size_request(48,-1);
	widget_interpolation=manage(new Widget_Enum());
	widget_interpolation->set_size_request(0, -1);
	widget_interpolation->show();
	widget_interpolation->signal_changed().connect(sigc::mem_fun(*this,&studio::Widget_Interpolation::on_interpolation_changed));
	widget_interpolation->set_param_desc(
		ParamDesc("interpolation")
			.set_hint("enum")
			.add_enum_value(INTERPOLATION_CLAMPED,"clamped",_("Clamped"))
			.add_enum_value(INTERPOLATION_TCB,"auto",_("TCB"))
			.add_enum_value(INTERPOLATION_CONSTANT,"constant",_("Constant"))
			.add_enum_value(INTERPOLATION_HALT,"ease",_("Ease In/Out"))
			.add_enum_value(INTERPOLATION_LINEAR,"linear",_("Linear"))
	);
	widget_interpolation->set_icon(0, Gtk::Button().render_icon(Gtk::StockID("synfig-interpolation_type_clamped"),Gtk::ICON_SIZE_MENU));
	widget_interpolation->set_icon(1, Gtk::Button().render_icon(Gtk::StockID("synfig-interpolation_type_tcb"),Gtk::ICON_SIZE_MENU));
	widget_interpolation->set_icon(2, Gtk::Button().render_icon(Gtk::StockID("synfig-interpolation_type_const"),Gtk::ICON_SIZE_MENU));
	widget_interpolation->set_icon(3, Gtk::Button().render_icon(Gtk::StockID("synfig-interpolation_type_ease"),Gtk::ICON_SIZE_MENU));
	widget_interpolation->set_icon(4, Gtk::Button().render_icon(Gtk::StockID("synfig-interpolation_type_linear"),Gtk::ICON_SIZE_MENU));
	synfigapp::Main::set_interpolation(INTERPOLATION_CLAMPED); // Clamped by default.

	attach(*widget_interpolation,0, 2, 5, 6, Gtk::EXPAND|Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 1, 1);
	widget_interpolation->set_tooltip_text(_("Default Interpolation"));

	// Signals
	synfigapp::Main::signal_interpolation_changed().connect(sigc::mem_fun(*this,&studio::Widget_Interpolation::interpolation_refresh));

	interpolation_refresh();

}

Widget_Interpolation::~Widget_Interpolation()
{
}


void
Widget_Interpolation::interpolation_refresh()
{
	widget_interpolation->set_value(synfigapp::Main::get_interpolation());
}


void
Widget_Interpolation::on_interpolation_changed()
{
	synfigapp::Main::set_interpolation(Waypoint::Interpolation(widget_interpolation->get_value()));
}

