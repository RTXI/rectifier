/*
			Copyright (C) 2011 Georgia Institute of Technology, University of Utah, Weill Cornell Medical College

			This program is free software: you can redistribute it and/or modify
			it under the terms of the GNU General Public License as published by
			the Free Software Foundation, either version 3 of the License, or
			(at your option) any later version.

			This program is distributed in the hope that it will be useful,
			but WITHOUT ANY WARRANTY; without even the implied warranty of
			MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
			GNU General Public License for more details.

			You should have received a copy of the GNU General Public License
			along with this program.  If not, see <http://www.gnu.org/licenses/>.
			*/

#include <rectifier.h>
#include <main_window.h>
#include <iostream>
#include <cmath>

extern "C" Plugin::Object *createRTXIPlugin(void){
	return new Rectifier();
}

static DefaultGUIModel::variable_t vars[] = {
	{ "Signal In", "Signal to rectify", DefaultGUIModel::INPUT, }, 
	{ "Signal Out", "Rectified signal", DefaultGUIModel::OUTPUT, }, 
	{ "Gain", "Optional gain of rectified output", DefaultGUIModel::PARAMETER }, 
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

Rectifier::Rectifier(void) : DefaultGUIModel("Rectifier", ::vars, ::num_vars) {
	setWhatsThis("<p><b>Rectifier:</b><br>QWhatsThis description.</p>");
	DefaultGUIModel::createGUI(vars, num_vars);
	customizeGUI();
	update(INIT);
	refresh();
	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

Rectifier::~Rectifier(void) { }

void Rectifier::execute(void) {
	switch (rectifier_type) {
		case half:
			output(0) = (input(0) < 0) ? 0 : gain*input(0);
			break;
		case full:
			output(0) = (input(0) < 0) ? gain*std::abs(input(0)) : gain*input(0);
			break;
		default:
			break;
	}
	return;
}

void Rectifier::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			rectifier_type = half;
			gain = 1.0;
			HalfButton->setDown(true);
			break;

		case MODIFY:
			gain = getParameter("Gain").toDouble();
			if (rectifier_type == half)
				HalfButton->setDown(true);
			else if(rectifier_type == full)
				FullButton->setDown(true);
			break;

		case UNPAUSE:
			break;

		case PAUSE:
			break;

		case PERIOD:
			period = RT::System::getInstance()->getPeriod() * 1e-6; // ms
			break;

		default:
			break;
	}
}

void Rectifier::setHalf() {
	rectifier_type = half;
	HalfButton->setDown(true);
	FullButton->setDown(false);
}

void Rectifier::setFull() {
	rectifier_type = full;
	HalfButton->setDown(false);
	FullButton->setDown(true);
}

void Rectifier::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	QGroupBox *button_group = new QGroupBox;
	HalfButton = new QPushButton("Half Wave");
	FullButton = new QPushButton("Full Wave");
	QHBoxLayout *button_layout = new QHBoxLayout;
	button_group->setLayout(button_layout);
	button_layout->addWidget(HalfButton);
	button_layout->addWidget(FullButton);
	QObject::connect(HalfButton, SIGNAL(released()), this, SLOT(setHalf()));
	QObject::connect(FullButton, SIGNAL(released()), this, SLOT(setFull()));
	customlayout->addWidget(button_group, 0,0);
	setLayout(customlayout);
}
