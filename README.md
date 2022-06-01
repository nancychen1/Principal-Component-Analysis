# Principal Component Analysis
 Subjects the 30 components of the Dow Jones Industrial Average to Principal Component Analysis


Using the time series of the daily percent price change for each of the 30 stocks in DJIA for the year of 2020 (253 trading days), the sample covariance and correlation matrices are computed and then the eigenvalues and eigenvectors of the covariance matrix are found. The first eigenvector (the one with the largest eigenvalue) is then used to create some graphs. The point of this assignment is to interpret the first eigenvector. 

The code and tex files produce a scatter plot of the stock's beta compared to their corresponding component in the first eigenvector. It also produces a scattterplot of the stock's beta multiplied by their total dow return on day t versus the first principal component of the t-th row of the covariance matrix multiplied by the i-th component of the first eigenvector. 

Based on the following plots shown in Prob3.png and Prob4.png, the conclusion is that the first eigenvector captures each stock'sresponse to changes in the overall market.
