
-- VHDL Test Bench Created from source file RAM_CTRL.vhd -- Wed Aug 01 18:29:50 2018

--
-- Notes: 
-- 1) This testbench template has been automatically generated using types
-- std_logic and std_logic_vector for the ports of the unit under test.
-- Lattice recommends that these types always be used for the top-level
-- I/O of a design in order to guarantee that the testbench will bind
-- correctly to the timing (post-route) simulation model.
-- 2) To use this template as your testbench, change the filename to any
-- name of your choice with the extension .vhd, and use the "source->import"
-- menu in the ispLEVER Project Navigator to import the testbench.
-- Then edit the user defined section below, adding code to generate the 
-- stimulus for your design.
-- 3) VHDL simulations will produce errors if there are Lattice FPGA library 
-- elements in your design that require the instantiation of GSR, PUR, and
-- TSALL and they are not present in the testbench. For more information see
-- the How To section of online help.  
--
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY RAM_CTRL_TB IS
END RAM_CTRL_TB;

ARCHITECTURE behavior OF RAM_CTRL_TB IS 

	COMPONENT RAM_CTRL
	PORT(
		clk : IN std_logic;
		write_clk : IN std_logic;
		RAM_A : IN std_logic_vector(14 downto 0);
		RAM_W : IN std_logic;
		RAM_D : IN std_logic_vector(15 downto 0);    
		RAM_EXT_D : INOUT std_logic_vector(15 downto 0);      
		RAM_Q : OUT std_logic_vector(15 downto 0);
		RAM_EXT_A : OUT std_logic_vector(14 downto 0);
		RAM_EXT_W : OUT std_logic;
		RAM_EXT_NOE : OUT std_logic
		);
	END COMPONENT;

	SIGNAL clk :  std_logic := '0';
	SIGNAL write_clk :  std_logic := '0';
	SIGNAL RAM_A :  std_logic_vector(14 downto 0);
	SIGNAL RAM_W :  std_logic := '0';
	SIGNAL RAM_D :  std_logic_vector(15 downto 0) := x"6BA9";
	SIGNAL RAM_Q :  std_logic_vector(15 downto 0);
	SIGNAL RAM_EXT_D :  std_logic_vector(15 downto 0);
	SIGNAL RAM_EXT_A :  std_logic_vector(14 downto 0);
	SIGNAL RAM_EXT_W :  std_logic;
	SIGNAL RAM_EXT_NOE :  std_logic;

BEGIN

-- Please check and add your generic clause manually
	uut: RAM_CTRL PORT MAP(
		clk => clk,
		write_clk => write_clk,
		RAM_A => RAM_A,
		RAM_W => RAM_W,
		RAM_D => RAM_D,
		RAM_Q => RAM_Q,
		RAM_EXT_D => RAM_EXT_D,
		RAM_EXT_A => RAM_EXT_A,
		RAM_EXT_W => RAM_EXT_W,
		RAM_EXT_NOE => RAM_EXT_NOE
	);

	clock : process
	begin				 
		wait for 2 ns;
		clk <= not clk;
		if (clk = '1') then
			write_clk <= not write_clk;
		end if;		  
	end process;

-- *** Test Bench - User Defined Section ***
	RAM_EXT_D <= x"3F86" when (RAM_EXT_NOE = '0') else (others => 'Z');
		
   tb : PROCESS
   BEGIN
	  wait for 40 ns;
	  RAM_W <= '1';
	  wait for 8 ns;
	  RAM_W <= '0';
	   
      wait; -- will wait forever
   END PROCESS;
-- *** End Test Bench - User Defined Section ***

END;
