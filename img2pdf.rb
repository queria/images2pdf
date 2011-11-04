#!/usr/bin/env ruby

require 'rubygems'
require 'pathname'
require 'prawn'

class Images2PDF
	attr_accessor :callback, :callback_finishing, :callback_finished

	def initialize(source_dir, target_pdf, verbose_output=false)
		@source = Pathname.new(source_dir)
		@target = Pathname.new(target_pdf)

		@verbose = verbose_output
		@aborted = false

		@images = []
		@processed = []

		@pdf = nil

		@callback = Proc.new { |total,done,file| puts "#{done}/#{total} processed #{file}" }
		@callback_finishing = Proc.new { puts "... saving and exiting" }
		@callback_finished = Proc.new { puts "all done" }
	end

	def puts(txt)
		return if not @verbose
		Kernel.puts txt
	end
	
	def start
		puts "Preparing conversion ..."

		suffixes = Regexp.new(/(?:jpg|jpeg|png)$/i)

		puts "- searching images in #{@source}:"
		@source.children.each do |child|
			next if not child.file?
			next if not suffixes.match( child.to_s )
			puts "- - found #{child}"
			@images << child
		end
		@pdf = Prawn::Document.new :page_size => 'A4', :margin => 0, :skip_page_creation => true, :compress => true, :optimize_objects => true
		puts "- going to save into #{@target}"
		process
	end

	private
	def process
		begin
			cnt = 0
			while (not @aborted) and (@processed.length < @images.length)
				current = @images[ @processed.length ]

				@pdf.start_new_page
				@pdf.image current.to_s, :at => [0, 841], :fit => [595, 840]
				@pdf.text current.to_s

				@processed << current
				@callback.call(@images.length, @processed.length, current) unless @callback.nil?
				cnt += 1
			end
		rescue Interrupt
			puts "Interrupted ... "
		end
		finish
	end

	private
	def finish
		@callback_finishing.call unless @callback_finishing.nil?
		@pdf.render_file @target
		@callback_finished.call unless @callback_finished.nil?
	end
end

if __FILE__ == $0
	if ARGV.length < 2 or ARGV.include? '-h' or ARGV.include? '--help'
		puts <<-EOHELP
use this as:
$ img2pdf <source_dir> <target_pdf>
for example
$ img2pdf /path/to/my/images_dir/ /path/to/my/exported.pdf
		EOHELP
		exit
	end

	i2p = Images2PDF.new(ARGV[0], ARGV[1], true)
	i2p.start
end
