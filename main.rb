#!/usr/bin/env ruby
require 'rubygems'
require 'Qt4'
require 'gui/mainwindow'
require 'img2pdf'


class String
	def qtr
		return Qt::Object.tr(self)
	end
end

app = Qt::Application.new(ARGV)

mw = MainWindow.new()
mw.show

app.exec

