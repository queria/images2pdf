

class InvalidFileInputMode < StandardError
	def initialize
		super('You have specified invalid FileInput mode')
	end
end

class FileInput < Qt::Widget
	MODE_DIR = 1
	MODE_FOPEN = 2
	MODE_FSAVE = 3


	signals :value_changed

	attr_accessor :dialog_caption, :dialog_dir, :dialog_filter

	def initialize(mode, parent=nil)
		super parent

		if mode < FileInput::MODE_DIR or mode > FileInput::MODE_FSAVE
			Kernel.raise InvalidFileInputMode.new
		end
		@mode = mode

		@dialog_caption = ''
		@dialog_dir = ''
		@dialog_filter = ''

		@layout = Qt::HBoxLayout.new
		@layout.set_margin 0
		set_layout @layout

		@text= Qt::LineEdit.new
		@text.connect(SIGNAL('textChanged(QString)')) { self.value_changed }
		@layout.add_widget(@text)

		@browse = Qt::PushButton.new('Browse'.qtr)
		@browse.connect(SIGNAL :clicked) { self.browse }
		@layout.add_widget(@browse)
	end

	def set_value(text)
		@text.text = text
		self.value_changed
	end
	def value=(text)
		self.set_value(text)
	end

	def value
		@text.text
	end

	def readable_dir?
		p = Pathname.new(value)
		return ( p.exist? and p.directory? and p.readable? )
	end

	def writable_file?
		p = Pathname.new(value)
		return ( p.parent.writable? and ( (not p.exist?) or p.writable? ) )
	end

	def browse
		path = case @mode
			   when FileInput::MODE_DIR
				   Qt::FileDialog::getExistingDirectory self, @dialog_caption, @dialog_dir
			   when FileInput::MODE_FOPEN
				   Qt::FileDialog::get_open_file_name self, @dialog_caption, @dialog_dir, @dialog_filter
			   when FileInput::MODE_FSAVE
				   Qt::FileDialog::get_save_file_name self, @dialog_caption, @dialog_dir, @dialog_filter
			   end
		if path.nil? or path.empty?
			return # cancelled
		end
		@text.text = path
		self.value_changed
	end
	
	private
	def changed
	end

end
