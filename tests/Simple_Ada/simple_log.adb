with Ada.Text_IO;

package body Simple_Log is

   use Ada.Text_IO;

   --
   Current_Output_Level : Output_Level_Type := Info;

   --
   procedure Set_Output (New_Output_Level : in Output_Level_Type) is
   begin
      Current_Output_Level := New_Output_Level;
   end Set_Output;

   --
   procedure Error (Message : in String) is
   begin
      if Current_Output_Level >= Error then
         Put_Line (Standard_Error, Message);  --  For errors
         --  Comments should be ignored, also as lines in between
      end if;
   end Error;

   --
   procedure Warning (Message : in String) is
   begin
      if Current_Output_Level >= Warning then
         Put_Line (Standard_Error, Message);  --  For warnings
      end if;
   end Warning;

   --
   procedure Info (Message : in String) is
   begin
      if Current_Output_Level >= Info then
         Put_Line (Standard_Error, Message);  --  For info
      end if;
   end Info;

   --
   procedure Debug (Message : in String) is
   begin
      if Current_Output_Level >= Debug then
         Put_Line (Standard_Error, Message);  --  For debug
      end if;
   end Debug;

end Simple_Log;
