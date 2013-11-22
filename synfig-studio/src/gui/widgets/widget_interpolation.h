/* === S Y N F I G ========================================================= */
/*!	\file widgets/widget_defaults.h
**	\brief Template Header
**
**	$Id$
**
**	\legal
**	Copyright (c) 2002-2005 Robert B. Quattlebaum Jr., Adrian Bentley
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

/* === S T A R T =========================================================== */

#ifndef __SYNFIG_STUDIO_WIDGET_DEFAULTS_H
#define __SYNFIG_STUDIO_WIDGET_DEFAULTS_H

/* === H E A D E R S ======================================================= */

#include <gtkmm/drawingarea.h>
#include <gtkmm/table.h>
#include <gtkmm/tooltip.h>

/* === M A C R O S ========================================================= */

/* === T Y P E D E F S ===================================================== */

/* === C L A S S E S & S T R U C T S ======================================= */

namespace Gtk { class HScale; }

namespace studio {

	class Widget_Enum;

	class Widget_Interpolation : public Gtk::Table
	{
		Widget_Enum	*widget_interpolation;
		void interpolation_refresh();
		void on_interpolation_changed();

	public:

		Widget_Interpolation();
		~Widget_Interpolation();
	}; // END of class BlackLevelSelector
}; // END of namespace studio

/* === E N D =============================================================== */

#endif
