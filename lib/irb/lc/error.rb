#
#   irb/lc/error.rb - 
#   	$Release Version: 0.7.3$
#   	$Revision$
#   	$Date$
#   	by Keiju ISHITSUKA(keiju@ishitsuka.com)
#
# --
#
#   
#
require "e2mmap"

module IRB

  # exceptions
  extend Exception2MessageMapper
  def_exception :UnrecognizedSwitch, "Unrecognized switch: %s"
  def_exception :NotImplementError, "Need to define `%s'"
  def_exception :CantReturnToNormalMode, "Can't return to normal mode."
  def_exception :IllegalParameter, "Illegal parameter(%s)."
  def_exception :IrbAlreadyDead, "Irb is already dead."
  def_exception :IrbSwitchedToCurrentThread, "Switched to current thread."
  def_exception :NoSuchJob, "No such job(%s)."
  def_exception :CantShiftToMultiIrbMode, "Can't shift to multi irb mode."
  def_exception :CantChangeBinding, "Can't change binding to (%s)."
  def_exception :UndefinedPromptMode, "Undefined prompt mode(%s)."

end
