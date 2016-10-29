/**
 * Copyright (C) 2016 Ikey Doherty
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *  
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <budgie-desktop/plugin.h>
#include <gobject/gobject.h>

#include "AppIndicatorApplet.h"

G_BEGIN_DECLS

void
load_modules (GtkWidget *menubar, gint *indicators_loaded);

void
load_indicators_from_indicator_files (GtkWidget *menubar, gint *indicators_loaded);

gboolean
menubar_on_draw (GtkWidget * widget,
                 cairo_t * cr,
                 GtkWidget * menubar);

static gchar * indicator_order[] = {
	"libapplication.so",
	"libmessaging.so",
	"libsoundmenu.so",
	"libdatetime.so",
	"libsession.so",
	NULL
};

static GtkPackDirection packdirection;

#define  MENU_DATA_INDICATOR_OBJECT  "indicator-object"
#define  MENU_DATA_INDICATOR_ENTRY   "indicator-entry"

#define  IO_DATA_ORDER_NUMBER        "indicator-order-number"


/**
 * Define our type, which is a BudgieApplet extension
 */
#define NATIVE_TYPE_PANEL_APPLET native_panel_applet_get_type ()
G_DECLARE_FINAL_TYPE(AppIndicatorPanelApplet, native_panel_applet, NATIVE, PANEL_APPLET, BudgieApplet)

G_END_DECLS

/**
 * Pass properties here too if you wish
 */
BudgieApplet *native_panel_applet_new(void)
{
        return BUDGIE_APPLET(g_object_new(NATIVE_TYPE_PANEL_APPLET, NULL));
}

/**
 * Simple instance tracking
 */
struct _AppIndicatorPanelApplet
{
        BudgieApplet parent;
};

/**
 * Initialise this applet instance. For now just throw on a label :)
 */
static void native_panel_applet_init(AppIndicatorPanelApplet *self)
{
        GtkWidget *label = NULL;
        GtkWidget *eventbox = NULL;
        GtkWidget *menubar = NULL;
        GtkCssProvider *css_provider;
        
        gint indicators_loaded = 0;
        
        menubar = gtk_menu_bar_new();
        
        //css_provider = gtk_css_provider_new ();
        //gtk_css_provider_load_from_data (css_provider, "#indicator-button { -GtkWidget-focus-padding: 0; -GtkWidget-focus-line-width: 0; -GtkButton-default-border: 0; -GtkButton-inner-border: 0; padding: 1px; border-width: 1px;}", -1, NULL);
        //gtk_style_context_add_provider (GTK_STYLE_CONTEXT (gtk_widget_get_style_context (GTK_WIDGET (menubar))), GTK_STYLE_PROVIDER (css_provider), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

        eventbox = gtk_event_box_new();
        gtk_container_add(GTK_CONTAINER(self), eventbox);

    
        /*gtk_rc_parse_string (
	    "style \"indicator-applet-style\"\n"
        "{\n"
        "    GtkMenuBar::shadow-type = none\n"
        "    GtkMenuBar::internal-padding = 0\n"
        "    GtkWidget::focus-line-width = 0\n"
        "    GtkWidget::focus-padding = 0\n"
        "}\n"
	    "style \"indicator-applet-menubar-style\"\n"
        "{\n"
        "    GtkMenuBar::shadow-type = none\n"
        "    GtkMenuBar::internal-padding = 0\n"
        "    GtkWidget::focus-line-width = 0\n"
        "    GtkWidget::focus-padding = 0\n"
        "    GtkMenuItem::horizontal-padding = 0\n"
        "}\n"
	    "style \"indicator-applet-menuitem-style\"\n"
        "{\n"
        "    GtkWidget::focus-line-width = 0\n"
        "    GtkWidget::focus-padding = 0\n"
        "    GtkMenuItem::horizontal-padding = 0\n"
        "}\n"
        "widget \"*.fast-user-switch-applet\" style \"indicator-applet-style\""
        "widget \"*.fast-user-switch-menuitem\" style \"indicator-applet-menuitem-style\""
        "widget \"*.fast-user-switch-menubar\" style \"indicator-applet-menubar-style\"");
        gtk_widget_set_name(GTK_WIDGET (eventbox), "fast-user-switch-applet");
        */
        gtk_widget_set_can_focus (menubar, TRUE);
        //gtk_widget_set_name(GTK_WIDGET (menubar), "fast-user-switch-menubar");

        g_signal_connect_after(menubar, "draw", G_CALLBACK(menubar_on_draw), menubar);
        gtk_container_set_border_width(GTK_CONTAINER(menubar), 0);
        
        //gtk_container_add(GTK_CONTAINER(eventbox), label);

        load_modules(menubar, &indicators_loaded);
        //load_indicators_from_indicator_files(menubar, &indicators_loaded);
        if (indicators_loaded == 0) {
            /* A label to allow for click through */
            GtkWidget * item = gtk_label_new(_("No Indicators"));
            gtk_container_add(GTK_CONTAINER(eventbox), item);
            gtk_widget_show(item);
        } else {
            gtk_container_add(GTK_CONTAINER(eventbox), menubar);
            gtk_widget_show(menubar);
        }
        
        /* Show all of our things. */
        gtk_widget_show_all(GTK_WIDGET(self));
}

/**
 * Unused in our implementation. Feel free to override class methods of
 * BudgieApplet here.
 */
static void native_panel_applet_class_init(__budgie_unused__ AppIndicatorPanelAppletClass *cls)
{
}

static void native_panel_applet_class_finalize(__budgie_unused__ AppIndicatorPanelAppletClass *cls)
{
}

/**
 * This is us now doing the implementation chain ups..
 */

G_DEFINE_DYNAMIC_TYPE_EXTENDED(AppIndicatorPanelApplet, native_panel_applet, BUDGIE_TYPE_APPLET, 0, )

/**
 * Work around the register types issue.
 */
void native_panel_applet_init_gtype(GTypeModule *module)
{
        native_panel_applet_register_type(module);
}
