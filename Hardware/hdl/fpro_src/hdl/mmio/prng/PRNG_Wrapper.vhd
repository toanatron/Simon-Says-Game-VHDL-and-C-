library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity PRNG_Wrapper is
Port (clk     : in  std_logic;
      reset   : in  std_logic;
      -- slot interface 
      cs      : in  std_logic;
      write   : in  std_logic;
      read    : in  std_logic;
      addr    : in  std_logic_vector(4 downto 0);
      rd_data : out std_logic_vector(31 downto 0);
      wr_data : in  std_logic_vector(31 downto 0) 
); 
end PRNG_Wrapper;

architecture Behavioral of PRNG_Wrapper is
signal i_seed: std_logic_vector(7 downto 0);
signal init_prng, ld_prng, en_prng: std_logic;
signal prng_out: std_logic_vector(1 downto 0);
signal to_rd_data: std_logic_vector(1 downto 0);
signal wr_en: std_logic;

begin
    prng_com: entity work.PRNG(Behavioral)
    port map(clk => clk, i_seed => i_seed, init_prng => init_prng, ld_prng => ld_prng, en_prng => en_prng, dout => prng_out);
process(clk)
begin
    if reset = '1' then
        init_prng <= '1';
        ld_prng <= '1';
        en_prng <= '1';
    elsif rising_edge(clk) then
        if wr_en = '1' then
            to_rd_data <= prng_out;
        end if;
        init_prng <= wr_data(28);
        ld_prng <= wr_data(24);
        en_prng <= wr_data(20);
    end if;
end process;

i_seed <= wr_data(7 downto 0);

wr_en <= '1' when cs = '1' and write = '1' else '0';
rd_data <= x"0000000" & "00" & to_rd_data;

end Behavioral;
