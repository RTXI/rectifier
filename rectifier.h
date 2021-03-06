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

#include <default_gui_model.h>

class Rectifier : public DefaultGUIModel {

	Q_OBJECT

	public:
		Rectifier(void);
		virtual ~Rectifier(void);

		void execute(void);
		void createGUI(DefaultGUIModel::variable_t *, int);
		void customizeGUI(void);

	protected:
		virtual void update(DefaultGUIModel::update_flags_t);

	private:
		enum rectifier_t {
			half,
			full
		};
		rectifier_t rectifier_type;
		double period;
		double gain;
		QPushButton *HalfButton;
		QPushButton *FullButton;

		private slots:
			void setHalf();
		void setFull();
};
