
#require 'gui/customer'
#require 'gui/invoice'
#require 'data/storage'
require 'gui/commons'

class MainWindow < Qt::MainWindow

	def initialize()
		super()

		if Qt::Icon::theme_name.eql? "hicolor"
			Qt::Icon::set_theme_name('Tango')
		end

		@i2p = nil

		init_actions
		init_gui
		init_connections
		revalidate
	end

	protected
	def init_actions
		@action_quit = Qt::Action.new(Qt::Icon::fromTheme('exit'), 'Quit', self)
		@action_quit.connect(SIGNAL :triggered) { $qApp.quit }
	end

	def init_gui
		self.set_minimum_size(600,300)
		
		menu_app = self.menu_bar.add_menu('Images2PDF'.qtr)
		menu_app.add_action(@action_quit)

		widget_main = Qt::Widget.new(self)
		layout_main = Qt::FormLayout.new(widget_main)

		@folder = FileInput.new FileInput::MODE_DIR
		@folder.dialog_caption = 'Images directory'.qtr
		@file = FileInput.new FileInput::MODE_FSAVE
		@file.dialog_caption = 'Save as PDF'.qtr

		@folder.value = '/tmp/i2p/small'
		@file.value = '/tmp/res.pdf'

		@process_button = Qt::PushButton.new('Merge'.qtr)

		@status = Qt::Label.new

		@progress = Qt::ProgressBar.new
		@progress.minimum = 0
		@progress.visible = false

		layout_main.addRow('Folder with images:', @folder)
		layout_main.addRow('Target pdf file:', @file)
		layout_main.addRow(@process_button)
		layout_main.addRow(@status)
		layout_main.addRow(@progress)

		self.setCentralWidget(widget_main)
	end

	def init_connections
		@folder.connect(SIGNAL :value_changed) { self.revalidate }
		@file.connect(SIGNAL :value_changed) { self.revalidate }
		@process_button.connect(SIGNAL :clicked) { self.process_start }
	end

	def revalidate
		if @folder.readable_dir? and @file.writable_file?
			@status.text = 'You can press Merge button now.'.qtr
			@process_button.enabled = true
		else
			@status.text = 'Select path to dir with images and where to save pdf.'.qtr
			@process_button.enabled = false
		end
	end

	def process_start
		@status.text = 'Preparing'.qtr

		@progress.value = 0
		@progress.maximum = 1
		@progress.visible = true

		@folder.enabled = false
		@file.enabled = false
		@process_button.enabled = false

		@i2p = Images2PDF.new(@folder.value, @file.value)
		@i2p.callback = Proc.new { |tot, done, last| self.process_step(tot, done, last) }
		@i2p.callback_finishing = Proc.new { self.process_rendering }
		@i2p.callback_finished = Proc.new { self.process_end }
		@i2p.start
	end

	def process_step(total, finished, current_file)
		@progress.maximum = total
		@progress.value = finished
		@status.text = "Added #{current_file}"
	end

	def process_rendering
		@progress.visible = false
		@status.text = "Rendering pdf please wait ..."
	end

	def process_end
		@i2p = nil

		@progress.visible = false

		@folder.enabled = true
		@file.enabled = true
		@process_button.enabled = false

		@status.text = 'All done. Hope you are satisfied with results.'
	end

end

