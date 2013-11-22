/* === S Y N F I G ========================================================= */
/*!	\file widget_opacity.cpp
**	\brief Template File
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
**  Copyright (c) 2008 Chris Moore
**  Copyright (c) 2008, 2011, 2012 Carlos López
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

#include "widgets/widget_opacity.h"
#include "widgets/widget_color.h"
#include "widgets/widget_gradient.h"
#include "dialogs/dialog_color.h"
#include "dialogs/dialog_gradient.h"
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

#define GRADIENT_HEIGHT		16
#define DEFAULT_INCREMENT	(0.25)
#define DEFAULT_WIDTH		(synfig::Distance(3,synfig::Distance::SYSTEM_POINTS))

/* === G L O B A L S ======================================================= */

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Widget_Opacity::Widget_Opacity()
{
	//set_size_request(48,48+GRADIENT_HEIGHT+16);
	//set_size_request(48,-1);

	widget_opacity=manage(new Gtk::HScale(0.0f,1.01f,0.01f));
	widget_opacity->show();
	widget_opacity->set_digits(2);
	widget_opacity->set_value_pos(Gtk::POS_LEFT);
	widget_opacity->signal_value_changed().connect(sigc::mem_fun(*this,&studio::Widget_Opacity::on_opacity_changed));
	attach(*widget_opacity,0, 2, 3, 4, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 1, 1);
	widget_opacity->set_tooltip_text(_("Default Opacity"));

	// Signals
	synfigapp::Main::signal_opacity_changed().connect(sigc::mem_fun(*this,&studio::Widget_Opacity::opacity_refresh));
	opacity_refresh();
}

Widget_Opacity::~Widget_Opacity()
{
}


void
Widget_Opacity::opacity_refresh()
{
	widget_opacity->set_value(synfigapp::Main::get_opacity());
}

void
Widget_Opacity::on_opacity_changed()
{
	synfigapp::Main::set_opacity(widget_opacity->get_value());
}