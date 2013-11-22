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

#include "widgets/widget_defaults.h"
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

class studio::Widget_Brush : public Gtk::DrawingArea
{
public:
	Widget_Brush()
	{
		signal_expose_event().connect(sigc::mem_fun(*this, &studio::Widget_Brush::redraw));

		set_size_request(24,24);
		add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
		add_events(Gdk::BUTTON1_MOTION_MASK);

		synfigapp::Main::signal_outline_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Brush::queue_draw));
		synfigapp::Main::signal_fill_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Brush::queue_draw));
		synfigapp::Main::signal_bline_width_changed().connect(sigc::mem_fun(*this,&studio::Widget_Brush::queue_draw));
		studio::App::signal_instance_selected().connect(sigc::hide(sigc::mem_fun(*this,&studio::Widget_Brush::queue_draw)));
	}

	bool
	redraw(GdkEventExpose */*bleh*/)
	{
		//!Check if the window we want draw is ready
		Glib::RefPtr<Gdk::Window> window = get_window();
		if(!window) return false;

		Glib::RefPtr<Gdk::GC> gc(Gdk::GC::create(window));

		const int h(get_height());
		const int w(get_width());

		float pixelsize(0);
		if(App::get_selected_canvas_view())
		{
			const RendDesc& rend_desc(App::get_selected_canvas_view()->get_canvas()->rend_desc());
			pixelsize=synfigapp::Main::get_bline_width().get(Distance::SYSTEM_PIXELS,rend_desc);
		}
		else
		{
			RendDesc rend_desc;
			pixelsize=synfigapp::Main::get_bline_width().get(Distance::SYSTEM_PIXELS,rend_desc);
		}
		// Fill in the fill color
		render_color_to_window(window,Gdk::Rectangle(0,0,w,h),synfigapp::Main::get_fill_color());

/*
		gc->set_rgb_fg_color(colorconv_synfig2gdk(synfigapp::Main::get_fill_color()));
		gc->set_line_attributes(1,Gdk::LINE_SOLID,Gdk::CAP_BUTT,Gdk::JOIN_MITER);
		window->draw_rectangle(
			gc,
			true,	// Fill?
			0,0,	// x,y
			w,h	//w,h
		);
*/

		// Draw in the circle
		gc->set_rgb_fg_color(colorconv_synfig2gdk(synfigapp::Main::get_outline_color()));
		gc->set_function(Gdk::COPY);
		gc->set_line_attributes(1,Gdk::LINE_SOLID,Gdk::CAP_BUTT,Gdk::JOIN_MITER);
		window->draw_arc(
			gc,
			true,
			round_to_int(((float)w/2.0f)-pixelsize/2.0f),
			round_to_int(((float)h/2.0f)-pixelsize/2.0f),
			round_to_int(pixelsize+0.6),
			round_to_int(pixelsize+0.6),
			0,
			360*64
		);

		return true;
	}

	bool
	on_event(GdkEvent *event)
	{
//		const int x(static_cast<int>(event->button.x));
		const int y(static_cast<int>(event->button.y));

		const int h(get_height());
//		const int w(get_width());

		switch(event->type)
		{
			case GDK_MOTION_NOTIFY:
				break;
			case GDK_BUTTON_RELEASE:
				if(event->button.button==1) // Left click
				{
					Distance dist(synfigapp::Main::get_bline_width());

					if(y<h/2) // increase BLine size
					{
						dist+=DEFAULT_INCREMENT;
					}
					else // Decrease BLine size
					{
						dist-=DEFAULT_INCREMENT;
					}
					synfigapp::Main::set_bline_width(dist);
					return true;
				}
				if(event->button.button==3)
				{
					// right click on bline width
					synfigapp::Main::set_bline_width(DEFAULT_WIDTH);
					return true;
				}
				break;
			case GDK_SCROLL:
				{
					Distance dist(synfigapp::Main::get_bline_width());

					switch(event->scroll.direction){
						case GDK_SCROLL_UP:
						case GDK_SCROLL_RIGHT:
							dist+=DEFAULT_INCREMENT;
						break;
						case GDK_SCROLL_DOWN:
						case GDK_SCROLL_LEFT:
							dist-=DEFAULT_INCREMENT;
						break;
					}
					synfigapp::Main::set_bline_width(dist);
					return true;
				}
			default:
				break;
		}

		return false;
	}

};

/* === P R O C E D U R E S ================================================= */

/* === M E T H O D S ======================================================= */

Widget_Defaults::Widget_Defaults()
{
	//set_size_request(48,48+GRADIENT_HEIGHT+16);
	//set_size_request(48,-1);

	{
		Gtk::Table* subtable(manage(new Gtk::Table()));
		Gtk::Image* icon;

		// Outline Color
		widget_otln_color=manage(new Widget_Color());
		widget_otln_color->show();
		widget_otln_color->set_size_request(16,16);
		widget_otln_color->signal_clicked().connect(sigc::mem_fun(*this,&Widget_Defaults::on_otln_color_clicked));
		widget_otln_color->set_tooltip_text(_("Outline Color"));

		// Fill Color
		widget_fill_color=manage(new Widget_Color());
		widget_fill_color->show();
		widget_fill_color->set_size_request(16,16);
		widget_fill_color->signal_clicked().connect(sigc::mem_fun(*this,&Widget_Defaults::on_fill_color_clicked));
		widget_fill_color->set_tooltip_text(_("Fill Color"));

		// Swap button
		Gtk::Button* button_swap(manage(new Gtk::Button()));
		button_swap->show();
		button_swap->set_relief(Gtk::RELIEF_NONE);
		button_swap->set_border_width(0);
		icon=manage(new Gtk::Image(Gtk::StockID("synfig-swap_colors"),Gtk::IconSize(1)));
		icon->show();
		button_swap->add(*icon);
		//button_swap->get_child()->set_size_request(16/3,16/3);
		//button_swap->set_size_request(16/3,16/3);
		dynamic_cast<Gtk::Misc*>(button_swap->get_child())->set_padding(0,0);
		button_swap->signal_clicked().connect(sigc::mem_fun(*this,&Widget_Defaults::on_swap_color_clicked));
		button_swap->set_tooltip_text(_("Swap Fill and\nOutline Colors"));

		// Reset button
		Gtk::Button* button_reset(manage(new Gtk::Button()));
		button_reset->show();
		button_reset->set_relief(Gtk::RELIEF_NONE);
		button_reset->set_border_width(0);
		icon=manage(new Gtk::Image(Gtk::StockID("synfig-reset_colors"),Gtk::IconSize(1)));
		icon->show();
		button_reset->add(*icon);
		dynamic_cast<Gtk::Misc*>(button_reset->get_child())->set_padding(0,0);
		//button_reset->set_size_request(16/3,16/3);
		button_reset->signal_clicked().connect(sigc::mem_fun(*this,&Widget_Defaults::on_reset_color_clicked));
		button_reset->set_tooltip_text(_("Reset Colors to Black and White"));

		subtable->attach(*widget_otln_color, 0, 4, 0, 4, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
		subtable->attach(*widget_fill_color, 3, 7, 3, 7, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 0, 0);
		subtable->attach(*button_swap, 4, 7, 0, 3, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
		subtable->attach(*button_reset, 0, 3, 4, 7, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);

		subtable->set_size_request(36,36);
		subtable->set_homogeneous(true);
		subtable->show();

		attach(*subtable, 0, 1, 0, 2, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 1, 1);

	}

	widget_brush=manage(new Widget_Brush());
	widget_brush->show();
	widget_brush->set_size_request(36,36);
	widget_brush->set_tooltip_text(_("Brush Preview"));

	widget_bline_width=manage(new Widget_Distance());
	widget_bline_width->show();
	bline_width_refresh();
	widget_bline_width->set_digits(2);
	widget_bline_width->set_range(0,10000000);
	widget_bline_width->set_size_request(24,-1);
	widget_bline_width->signal_value_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::on_bline_width_changed));
	widget_bline_width->set_tooltip_text(_("Brush Size"));

	widget_gradient=manage(new Widget_Gradient());
	widget_gradient->show();
	widget_gradient->set_size_request(-1,GRADIENT_HEIGHT);
	widget_gradient->signal_clicked().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::on_gradient_clicked));
	widget_gradient->set_tooltip_text(_("Default Gradient"));

	attach(*widget_brush,1, 2, 0, 1, Gtk::FILL, Gtk::EXPAND|Gtk::FILL, 1, 1);
	attach(*widget_bline_width,1, 2, 1, 2, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 0, 0);
	attach(*widget_gradient,0, 2, 4, 5, Gtk::FILL, Gtk::SHRINK|Gtk::FILL, 1, 1);


	// Signals
	synfigapp::Main::signal_bline_width_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::bline_width_refresh));
	synfigapp::Main::signal_outline_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::otln_color_refresh));
	synfigapp::Main::signal_fill_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::fill_color_refresh));
	synfigapp::Main::signal_gradient_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::gradient_refresh));

	otln_color_refresh();
	fill_color_refresh();
	gradient_refresh();
/*
	set_size_request(48,48+GRADIENT_HEIGHT);
	signal_expose_event().connect(sigc::mem_fun(*this, &studio::Widget_Defaults::redraw));
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	add_events(Gdk::BUTTON1_MOTION_MASK);

	synfigapp::Main::signal_outline_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::queue_draw));
	synfigapp::Main::signal_fill_color_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::queue_draw));
	synfigapp::Main::signal_gradient_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::queue_draw));
	synfigapp::Main::signal_bline_width_changed().connect(sigc::mem_fun(*this,&studio::Widget_Defaults::queue_draw));

	if(App::dialog_gradient)
	{
		App::dialog_gradient->set_gradient(synfigapp::Main::get_gradient());
		App::dialog_gradient->reset();
		App::dialog_gradient->signal_edited().connect(sigc::mem_fun(synfigapp::Main::set_gradient));
	}

	if(App::dialog_color)
	{
		App::dialog_color->set_color(synfigapp::Main::get_outline_color());
		App::dialog_color->reset();
		App::dialog_color->signal_edited().connect(sigc::mem_fun(synfigapp::Main::set_outline_color));
	}
*/
}

Widget_Defaults::~Widget_Defaults()
{
}

void
Widget_Defaults::otln_color_refresh()
{
	widget_otln_color->set_value(synfigapp::Main::get_outline_color());
}

void
Widget_Defaults::fill_color_refresh()
{
	widget_fill_color->set_value(synfigapp::Main::get_fill_color());
}

void
Widget_Defaults::gradient_refresh()
{
	widget_gradient->set_value(synfigapp::Main::get_gradient());
}

void
Widget_Defaults::bline_width_refresh()
{
	widget_bline_width->set_value(synfigapp::Main::get_bline_width());
}


void
Widget_Defaults::on_bline_width_changed()
{
	synfigapp::Main::set_bline_width(widget_bline_width->get_value());
}

void
Widget_Defaults::on_otln_color_clicked()
{
	// Left click on outline color
	App::dialog_color->set_color(synfigapp::Main::get_outline_color());
	App::dialog_color->reset();
	App::dialog_color->signal_edited().connect(sigc::ptr_fun(synfigapp::Main::set_outline_color));
	App::dialog_color->present();
}

void
Widget_Defaults::on_fill_color_clicked()
{
	// Left click on fill color
	App::dialog_color->set_color(synfigapp::Main::get_fill_color());
	App::dialog_color->reset();
	App::dialog_color->signal_edited().connect(sigc::ptr_fun(synfigapp::Main::set_fill_color));
	App::dialog_color->present();
}

void
Widget_Defaults::on_swap_color_clicked()
{
	synfigapp::Main::color_swap();
}

void
Widget_Defaults::on_reset_color_clicked()
{
	synfigapp::Main::set_fill_color(Color::white());
	synfigapp::Main::set_outline_color(Color::black());
}

void
Widget_Defaults::on_gradient_clicked()
{
	App::dialog_gradient->set_gradient(synfigapp::Main::get_gradient());
	App::dialog_gradient->reset();
	App::dialog_gradient->signal_edited().connect(sigc::ptr_fun(synfigapp::Main::set_gradient));
	App::dialog_gradient->set_default_button_set_sensitive(false);
	App::dialog_gradient->present();
}


/*
bool
Widget_Defaults::redraw(GdkEventExpose*bleh)
{
	Glib::RefPtr<Gdk::GC> gc(Gdk::GC::create(get_window()));

	const int h(get_height());
	const int w(get_width());
	const int size=std::min(h-GRADIENT_HEIGHT,w);

	render_color_to_window(get_window(),Gdk::Rectangle(size/4,size/4,size/4*3-1,size/4*3-1),synfigapp::Main::get_fill_color());
	render_color_to_window(get_window(),Gdk::Rectangle(0,0,size/4*3-1,size/4*3-1),synfigapp::Main::get_outline_color());
	render_gradient_to_window(get_window(),Gdk::Rectangle(0,h-GRADIENT_HEIGHT,w,GRADIENT_HEIGHT-1),synfigapp::Main::get_gradient());





	Glib::RefPtr<Pango::Layout> layout(Pango::Layout::create(get_pango_context()));

	gc->set_rgb_fg_color(Gdk::Color("#FF0000"));
	layout->set_text(synfigapp::Main::get_bline_width().get_string(2));
	layout->set_alignment(Pango::ALIGN_CENTER);
	layout->set_width(w/2);
	get_window()->draw_layout(gc, w*3/4, (h-GRADIENT_HEIGHT)-16, layout);

	return true;
}

bool
Widget_Defaults::on_event(GdkEvent *event)
{
	const int x(static_cast<int>(event->button.x));
	const int y(static_cast<int>(event->button.y));

	const int h(get_height());
	const int w(get_width());
	const int size=std::min(h-GRADIENT_HEIGHT,w);

	switch(event->type)
	{
	case GDK_MOTION_NOTIFY:
		break;
	case GDK_BUTTON_PRESS:
//			if(event->button.button==1 && y>get_height()-CONTROL_HEIGHT)
		break;
	case GDK_BUTTON_RELEASE:
		if(event->button.button==1)
		{
			if(y>size)
			{
				// Left click on gradient
				App::dialog_gradient->set_gradient(synfigapp::Main::get_gradient());
				App::dialog_gradient->reset();
				App::dialog_gradient->signal_edited().connect(sigc::mem_fun(synfigapp::Main::set_gradient));
				App::dialog_gradient->present();
				return true;
			}
			if(x>0 && x<=size)
			{
				if(x<size*3/4 && y<size*3/4)
				{
					// Left click on outline coloe
					App::dialog_color->set_color(synfigapp::Main::get_outline_color());
					App::dialog_color->reset();
					App::dialog_color->signal_edited().connect(sigc::mem_fun(synfigapp::Main::set_outline_color));
					App::dialog_color->present();
					return true;
				}
				if(x>size*3/4 && y>size/4)
				{
					// Left click on fill color
					App::dialog_color->set_color(synfigapp::Main::get_fill_color());
					App::dialog_color->reset();
					App::dialog_color->signal_edited().connect(sigc::mem_fun(synfigapp::Main::set_fill_color));
					App::dialog_color->present();
					return true;
				}
			}
			if(x>size) // Left click on BLine Width
			{
				Distance dist(synfigapp::Main::get_bline_width());

				if(y<size/2) // increase BLine size
				{
					dist+=DEFAULT_INCREMENT;
				}
				else // Decrease BLine size
				{
					dist-=DEFAULT_INCREMENT;
				}
				synfigapp::Main::set_bline_width(dist);
			}
		}
		if(event->button.button==3)
		{
			if(y>size)
			{
				// right click on gradient
				synfigapp::Main::set_gradient_default_colors();
				return true;
			}
			else
			{
				if(x<size)
				{
					// right click on colors
					synfigapp::Main::color_swap();
					return true;
				}

				if(x>w/2)
				{
					// right click on bline width
					synfigapp::Main::set_bline_width(DEFAULT_WIDTH);
				}

			}
		}
		break;
	case GDK_SCROLL:
		{
			Distance dist(synfigapp::Main::get_bline_width());

			if(event->scroll.direction==GDK_SCROLL_UP)
			{
				dist+=DEFAULT_INCREMENT;
			}
			else if(event->scroll.direction==GDK_SCROLL_DOWN)
			{
				dist-=DEFAULT_INCREMENT;
			}
			synfigapp::Main::set_bline_width(dist);
		}
	default:
		break;
	}

	return false;
}
*/
