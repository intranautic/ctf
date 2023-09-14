FROM skysider/pwndocker:latest
LABEL maintainer="intranautic <intranautic@gmail.com>"

RUN apt install -y zip unzip zsh gettext
RUN chsh root -s /usr/bin/zsh
RUN git clone https://github.com/intranautic/ctf ~/ctf

# setup dotfiles
RUN git clone https://github.com/intranautic/dot /tmp/dot
RUN mkdir -p ~/.config && cp -r /tmp/dot/nvim ~/.config/nvim
RUN cp /tmp/dot/tmux.conf ~/.tmux.conf

# install zsh shell
RUN wget https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh
RUN chmod +x install.sh && ./install.sh --unattended
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git ~/powerlevel10k
RUN echo 'source ~/powerlevel10k/powerlevel10k.zsh-theme' >>~/.zshrc

# neovim build and install + plugins
RUN git clone https://github.com/neovim/neovim /tmp/neovim
RUN cd /tmp/neovim && make && make install

WORKDIR /
