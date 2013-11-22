/* === S Y N F I G ========================================================= */
/*!	\file widget_defaults.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**  Copyright (c) 2008 Chris Moore
**  Copyright (c) 2008, 2011, 2012 Carlos López
**	Coypright (c) 2013 Yu Chen
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

#include "widgets/widget_blendmethod.h"
#include "app.h"
#include <gtkmm/menu.h>
#include <gtkmm/scale.h>
#include <synfig/exception.h>
#include <synfigapp/main.h>
#include "canvasview.h"
#include "widgets/widget_enum.h"
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

Widget_Blend_Method::Widget_Blend_Method()
{
	//set_size_request(48,48+GRADIENT_HEIGHT+16);
	//set_size_request(48,-1);

	widget_blend_method=manage(new Widget_Enum());
	widget_blend_method->set_size_request(0, -1);
	widget_blend_method->show();
	widget_blend_method->signal_changed().connect(sigc::mem_fun(*this,&studio::Widget_Blend_Method::on_blend_method_changed));
	widget_blend_method->set_param_desc(
		ParamDesc(Color::BLEND_COMPOSITE,"blend_method")
		.add_enum_value(Color::BLEND_BY_LAYER,"bylayer", _("By Layer Default"))
	);
	attach(*widget_blend_method,0, 2, 2, 3, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 1, 1);
	widget_blend_method->set_tooltip_text(_("Default Blend Method"));

	// Signals
	synfigapp::Main::signal_blend_method_changed().connect(sigc::mem_fun(*this,&studio::Widget_Blend_Method::blend_method_refresh));

	blend_method_refresh();
}


Widget_Blend_Method::~Widget_Blend_Method()
{
}


void
Widget_Blend_Method::blend_method_refresh()
{
	widget_blend_method->set_value(synfigapp::Main::get_blend_method());
}


void
Widget_Blend_Method::on_blend_method_changed()
{
	synfigapp::Main::set_blend_method(Color::BlendMethod(widget_blend_method->get_value()));
}