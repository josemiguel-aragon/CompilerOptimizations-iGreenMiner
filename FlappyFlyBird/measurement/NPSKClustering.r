# Import libraries
library(ScottKnottESD)
library(readr)
library(ggplot2)

# load data
model_performance <- read_csv("ec_measurements_df.csv")
model_performance <- model_performance[c(2:length(model_performance))]

# apply ScottKnottESD and prepare a ScottKnottESD dataframe
sk_results <- sk_esd(model_performance)
sk_ranks <- data.frame(model = names(sk_results$groups),
                       rank = paste0('Rank-', sk_results$groups))

# prepare a dataframe for generating a visualisation
plot_data <- melt(model_performance)
plot_data <- merge(plot_data, sk_ranks, by.x = 'variable', by.y = 'model')

plot_data$variable <- as.character(plot_data$variable)

plot_data$variable <- paste("-", plot_data$variable, sep="")

# generate a visualisationz
g <- ggplot(data = plot_data, aes(x = variable, y = value)) +
  geom_boxplot(notch = FALSE, size = 0.25, # Líneas más delgadas
               outlier.shape = 1, # Forma de círculo hueco para los outliers
               outlier.size = 2, # Tamaño pequeño de los outliers
               outlier.stroke = 0.5) +
  stat_boxplot(geom ='errorbar', width = 0.25, size = 0.25) +
  facet_grid(~rank, scales = 'free_x') +
  scale_fill_brewer(direction = -1) +
  ylab('Energy consumption (J)') + xlab('') + ggtitle('') + theme_bw() +
  #theme(axis.line = element_line(color = "black", size = 0.5)) +
  theme(axis.title = element_text(size = 26, face = "plain")) +
  stat_summary(fun = median, geom = "crossbar", 
               width = 0.5, color = "lightcoral", size = 0.25) +
  theme(text = element_text(size = 24)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1)) +
  theme(panel.grid.major = element_line(color = "grey80", size = 0.0),
        panel.grid.minor = element_line(color = "grey90", size = 0.0)) +
  #theme(panel.border = element_blank()) +
  theme(axis.title.x = element_text(margin = margin(t = 10)),
        axis.title.y = element_text(margin = margin(r = 10))) +
  theme(legend.position = 'none')
#ggsave("ecRankingGenericRaspiPolybench.png")
g


# Import libraries
library(ScottKnottESD)
library(readr)
library(ggplot2)

# load data
model_performance <- read_csv("time_measurements_df.csv")
model_performance <- model_performance[c(2:length(model_performance))]

# apply ScottKnottESD and prepare a ScottKnottESD dataframe
sk_results <- sk_esd(model_performance)
sk_ranks <- data.frame(model = names(sk_results$groups),
                       rank = paste0('Rank-', sk_results$groups))

# prepare a dataframe for generating a visualisation
plot_data <- melt(model_performance)
plot_data <- merge(plot_data, sk_ranks, by.x = 'variable', by.y = 'model')

plot_data$variable <- as.character(plot_data$variable)

plot_data$variable <- paste("-", plot_data$variable, sep="")

# generate a visualisationz1
g <- ggplot(data = plot_data, aes(x = variable, y = value)) +
  geom_boxplot(notch = FALSE, size = 0.25, # Líneas más delgadas
               outlier.shape = 1, # Forma de círculo hueco para los outliers
               outlier.size = 2, # Tamaño pequeño de los outliers
               outlier.stroke = 0.5) +
  stat_boxplot(geom ='errorbar', width = 0.25, size = 0.25) +
  facet_grid(~rank, scales = 'free_x') +
  scale_fill_brewer(direction = -1) +
  ylab('Run time (s)') + xlab('') + ggtitle('') + theme_bw() +
  #theme(axis.line = element_line(color = "black", size = 0.5)) +
  theme(axis.title = element_text(size = 26, face = "plain")) +
  stat_summary(fun = median, geom = "crossbar", 
               width = 0.5, color = "lightcoral", size = 0.25) +
  theme(text = element_text(size = 24)) +
  theme(axis.text.x = element_text(angle = 45, hjust = 1)) +
  theme(panel.grid.major = element_line(color = "grey80", size = 0.0),
        panel.grid.minor = element_line(color = "grey90", size = 0.0)) +
  #theme(panel.border = element_blank()) +
  theme(axis.title.x = element_text(margin = margin(t = 10)),
        axis.title.y = element_text(margin = margin(r = 10))) +
  theme(legend.position = 'none')
#ggsave("timeRankingGenericRaspiPolybench.png")
g 
  
